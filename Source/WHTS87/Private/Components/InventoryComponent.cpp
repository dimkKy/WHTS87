// by Dmitry Kolontay


#include "Components/InventoryComponent.h"
#include "Environment/PickupItemContainer.h"
#include "Environment/Pickups/PickupItemInfoBase.h"

UInventoryComponent::UInventoryComponent() : XInventorySize{ 6 }, YInventorySize{ 5 }
{
	TArray<APickupItemContainer*> cellsColumnTemp;
	cellsColumnTemp.Init(nullptr, YInventorySize);
	cells.Init(cellsColumnTemp, XInventorySize);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UInventoryComponent::ChangeInventorySize(int32 newYInventorySize)
{
	if (newYInventorySize > YInventorySize) {
		for (auto& column : cells) {
			column.SetNum(newYInventorySize);
			for (int32 i{ YInventorySize }; i < newYInventorySize; ++i) {
				column[i] = nullptr;
			}
		}
		return true;
	}
	if (newYInventorySize < YInventorySize) {
		//TODO
		
		if (true) {
			for (auto& column : cells) {
				column.SetNum(newYInventorySize, false);
			}
			return true;
		}
		else {
			return false;
		}
		
	}
	return true;
	
	YInventorySize = newYInventorySize;
	
	//Cells.Init(FCellsColumn(YInventorySize), XInventorySize);
	//TArray<FItemContainer*> CellsColumnTemp;
	TArray<APickupItemContainer*> CellsColumnTemp;
	CellsColumnTemp.Init(nullptr, YInventorySize);
	cells.Init(CellsColumnTemp, XInventorySize);
	
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UInventoryComponent::AddContainer(APickupItemContainer* containerToAdd, bool bShouldTopUp, bool bBroadcast)
{
	if (IsValid(containerToAdd)) {
		int32 addedAmount{ 0 };
		int32 quantityToFit{ containerToAdd->GetItemsCount() };
		if (bShouldTopUp) {
			auto iterator{ containerMap.CreateConstIterator() };
			while (quantityToFit > 0 && iterator) {
				if (iterator.Key()->GetItemInfo()->GetClass() == containerToAdd->GetItemInfo()->GetClass()) {
					int32 fittedAmount{ std::min(quantityToFit, iterator.Key()->GetLackingItemsCount()) };
					quantityToFit -= fittedAmount;
					addedAmount += fittedAmount;
					containerToAdd->ChangeItemsCount(-fittedAmount);
					iterator.Key()->ChangeItemsCount(fittedAmount);
				}
				++iterator;
			}
		}
		if (quantityToFit > 0) {
			//TODO
			int32 XLimit{ XInventorySize - containerToAdd->GetItemInfo()->GetXInventorySize() };
			int32 YLimit{ YInventorySize - containerToAdd->GetItemInfo()->GetYInventorySize() };
			for (int32 YPosition{ 0 }; YPosition <= YLimit; YPosition++) {
				for (int32 XPosition = 0; XPosition <= XLimit; XPosition++) {
					if (TryInsertContainer(containerToAdd, XPosition, YPosition, false)) {
						addedAmount += containerToAdd->GetItemsCount();
						containerToAdd->SetOwnerInventory(this);
						if (onInventoryUpdated.IsBound() && bBroadcast)
							onInventoryUpdated.Broadcast();
						return addedAmount;
					}
				}
			}
		}
		if (onInventoryUpdated.IsBound() && bBroadcast && addedAmount > 0)
			onInventoryUpdated.Broadcast();
		return addedAmount;
	}
	else
		return 0;
}

bool UInventoryComponent::CheckInsertContainer(APickupItemContainer* containerToCheck, int32 XPosition, int32 YPosition)
{
	if (IsValid(containerToCheck)) {
		int32 YLimit{ containerToCheck->GetItemInfo()->GetYInventorySize() };
		int32 XLimit{ containerToCheck->GetItemInfo()->GetXInventorySize() };
		for (int32 Y{ 0 }; Y < YLimit; Y++) {
			for (int32 X{ 0 }; X < XLimit; X++) {
				if (cells[XPosition + X][YPosition + Y] != nullptr)
					return false;
			}
		}
		return true;
	}
	else
		return false;
}

bool UInventoryComponent::TryInsertContainer(APickupItemContainer* containerToTryInserting, int32 XPosition, int32 YPosition, bool bBroadcast)
{
	if (CheckInsertContainer(containerToTryInserting, XPosition, YPosition)) {
		int32 YLimit{ containerToTryInserting->GetItemInfo()->GetYInventorySize() };
		int32 XLimit{ containerToTryInserting->GetItemInfo()->GetXInventorySize() };
		for (int32 Y = 0; Y < YLimit; Y++) {
			for (int32 X = 0; X < XLimit; X++)
				cells[XPosition + X][YPosition + Y] = containerToTryInserting; //?
		}
		containerMap.Add(containerToTryInserting, TPair<int32, int32>{XPosition, YPosition});
		containerToTryInserting->SetOwnerInventory(this);
		if (bBroadcast && onInventoryUpdated.IsBound())
			onInventoryUpdated.Broadcast();
		return true;
	}
	else {
		return false;
	}
}

APickupItemContainer* UInventoryComponent::RemoveContainer(int32 XPosition, int32 YPosition, bool bBroadcast, bool bEject)
{
	if (RemoveContainer(cells[XPosition][YPosition], bBroadcast, bEject))
		return cells[XPosition][YPosition];
	else
		return nullptr;
}

bool UInventoryComponent::RemoveContainer(APickupItemContainer* containerToRemove, bool bBroadcast, bool bEject)
{
	TPair<int32, int32> containerPosition{ -999, -999 };
	containerMap.RemoveAndCopyValue(containerToRemove, containerPosition);
	if (containerPosition.Key == -999 || containerPosition.Value == -999) {
		//UE_LOG(LogTemp, Error, TEXT("Removing container has wrong coordinates: X is %d, Y is %d"), containerPosition.Key, containerPosition.Value);
		return false;
	}
	int32 YLimit{ containerToRemove->GetItemInfo()->GetYInventorySize() };
	int32 XLimit{ containerToRemove->GetItemInfo()->GetXInventorySize() };
	for (int32 Y{ 0 }; Y < YLimit; Y++) {
		for (int32 X{ 0 }; X < XLimit; X++) {
			cells[containerPosition.Key + X][containerPosition.Value + Y] = nullptr;
		}
	}
	containerToRemove->OnRemoveFromInventory(bEject);
	if (bBroadcast && onInventoryUpdated.IsBound())
		onInventoryUpdated.Broadcast();
	return true;
}

int32 UInventoryComponent::CheckItems(UPickupItemInfoBase* itemToFind, int32 requestedQuantity) const
{
	if (requestedQuantity < 1)
		return 0;
	int32 lackingQuantity{ requestedQuantity };
	auto iterator{ containerMap.CreateConstIterator() };
	while (lackingQuantity > 0 && iterator) {
		if (iterator.Key()->GetItemInfo()->GetClass() == itemToFind->GetClass()) {
			lackingQuantity -= iterator.Key()->GetItemsCount();
		}
		++iterator;
	}
	if (lackingQuantity < 1)
		return requestedQuantity;
	else {
		return requestedQuantity - lackingQuantity;
	}
}

bool UInventoryComponent::TryUseItems(UPickupItemInfoBase* itemToUse, int32 quantityToUse, bool bBroadcast)
{
	if (CheckItems(itemToUse, quantityToUse) == quantityToUse) {
		auto iterator{ containerMap.CreateConstIterator() };
		while (quantityToUse > 0 && iterator) {
			if (iterator.Key()->GetItemInfo()->GetClass() == itemToUse->GetClass()) {

				int32 availableAmount{ std::min(quantityToUse, iterator.Key()->GetItemsCount()) };
				//disposal
				iterator.Key()->ChangeItemsCount(-availableAmount);


				quantityToUse -= availableAmount;
			}
			++iterator;
		}

		return true;
	}
	else {
		return false;
	}
}

bool UInventoryComponent::GetContainerCoordinates(APickupItemContainer* pickedUpContainer, int32& outXPosition, int32& outYPosition) const
{
	auto coordinates{ containerMap.Find(pickedUpContainer) };
	if (coordinates != nullptr) {
		outXPosition = coordinates->Key;
		outYPosition = coordinates->Value;
		return true;
	}
	else
		return false;
}

