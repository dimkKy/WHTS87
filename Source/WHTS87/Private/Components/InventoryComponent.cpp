// by Dmitry Kolontay


#include "Components/InventoryComponent.h"
#include "Environment/PickupItemContainer.h"
#include "Environment/Pickups/PickupItemInfoBase.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UInventoryComponent::UInventoryComponent() : 
	xSize{ 6 }, ySize{ 5 }
{
	cells.Init(nullptr, ySize * ySize);
}

#if WITH_EDITOR
void UInventoryComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (ySize < minYsize) {
		ySize = minYsize;
	}
}
#endif

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

int32 UInventoryComponent::TryTopUpContainer(APickupItemContainer& target, APickupItemContainer& source, int32 quantity, bool bBroadcast)
{
	if (target.GetItemInfo() != source.GetItemInfo()) {
		return 0;
	}
	//?
	if (&target == &source) {
		return quantity;
	}
	if (quantity < 1) {
		quantity = source.GetItemsCount();
		if (quantity < 1) {
			return 0;
		}
	}
	int32 fittedAmount{ -1 * source.ChangeItemsCount(-1 * target.ChangeItemsCount(quantity)) };

	if (bBroadcast && fittedAmount > 0 && onInventoryUpdated.IsBound())
		onInventoryUpdated.Broadcast();
	return fittedAmount;
}

bool UInventoryComponent::CheckFreeCells(const FIntPoint& pos, const FIntPoint& size) const
{
	if (pos.X + size.X > xSize || pos.Y + size.Y > ySize) {
		return false;
	}
	for (FIntPoint offset{ 0 }; offset.Y < size.Y; ++offset.Y) {
		for (offset.X = 0; offset.X < size.X; ++offset.X) {
			if (cells[pos.X + offset.X + (pos.Y + offset.Y) * xSize]) {
				return false;
			}
		}
	}
	return true;
}

bool UInventoryComponent::ChangeInventorySize(int32 newYsize)
{
	if (newYsize > ySize) {
		//check how it works
		cells.SetNum(newYsize * xSize);
		for (int32 i{ ySize * xSize - 1 }; i < ySize * xSize; ++i) {
			cells[i]++;
		}
		return true;
	}
	if (newYsize < ySize && newYsize > minYsize) {
		for (int32 n{ newYsize * xSize }; n < cells.Num(); ++n) {
			if (cells[n]) {
				return false;
			}
		}
		ySize = newYsize;
		return true;
	}
	return true;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UInventoryComponent::TryAddContainer(APickupItemContainer& container, bool bShouldTopUp, bool bBroadcast)
{
	//?
	if (!IsValidChecked(&container) || container.GetItemsCount() < 1 || container.GetOwnerInventory() == this) {
		//todo
		check(false);
		return 0;
	}

	int32 addedAmount{ 0 };
	int32 quantityToFit{ container.GetItemsCount() };
	if (bShouldTopUp) {
		auto iterator{ containerMap.CreateConstIterator() };
		while (quantityToFit > 0 && iterator) {
			if (int32 fittedAmount{ TryTopUpContainer(*iterator.Key(), container, quantityToFit, false) }) {
				quantityToFit -= fittedAmount;
				addedAmount += fittedAmount;
			}
			++iterator;
		}
	}
	if (quantityToFit > 0) {
		FIntPoint limits{ xSize, ySize };
		limits -= container.GetItemInfo()->GetInventorySize();

		for (FIntPoint pos{ 0 }; pos.Y <= limits.Y; ++pos.Y) {
			for (; pos.X <= limits.X; ++pos.X) {
				if (int32 ret{ InsertNewContainer(container, pos) }) {
					addedAmount += ret;
					//container.SetOwnerInventory(this);
					pos.Y = limits.Y;
					break;
				}
			}
		}
	}
	else {
		//?
	}
	if (bBroadcast && addedAmount > 0 && onInventoryUpdated.IsBound())
		onInventoryUpdated.Broadcast();
	return addedAmount;
}

int32 UInventoryComponent::TryAddContainer(APickupItemContainer& container, const FIntPoint& pos, bool bShouldTopUp, bool bBroadcast)
{
	if (!IsValidChecked(&container) || container.GetItemsCount() < 1) {
		//todo
		check(false);
		return 0;
	}

	int32 targetCell{ pos.X + (pos.Y) * xSize };
	if (cells[targetCell]) {
		//possible top up
		if (bShouldTopUp) {
			return TryTopUpContainer(*cells[targetCell], container, 0, true);
		}
		else {
			return 0;
		}
	}
	else {
		int32 out{ 0 };
		if (container.GetOwnerInventory() == this) {
			out = MoveContainer(container, pos);
		}
		else {
			out = InsertNewContainer(container, pos);
		}

		if (bBroadcast && out && onInventoryUpdated.IsBound()) {
			onInventoryUpdated.Broadcast();
		}
		return out;
	}
}

int32 UInventoryComponent::CheckInsertContainer(APickupItemContainer& container, const FIntPoint& pos) const
{
	if (!IsValidChecked(&container)) {
		return 0;
	}
		
	int32 targetCell{ pos.X + (pos.Y) * xSize };
	if (cells[targetCell]) {
		//possible top up
		if (cells[targetCell]->GetItemInfo() == container.GetItemInfo()) {
			return cells[targetCell]->GetLackingItemsCount();
		}
		else {
			return 0;
		}	
	}
	else {
		//no top-up
		if (CheckFreeCells(pos, container.GetItemInfo()->GetInventorySize())) {
			return container.GetItemsCount();
		}
		else {
			return 0;
		}
	}
}

int32 UInventoryComponent::InsertNewContainer(APickupItemContainer& container, const FIntPoint& pos)
{
	if (CheckInsertContainer(container, pos) && container.RemoveFromInventory(false)) {
		containerMap.Add(&container, pos.X + (pos.Y) * xSize);
		container.SetOwnerInventory(*this);
		return container.GetItemsCount();
	}
	else {
		return 0;
	}
}

int32 UInventoryComponent::MoveContainer(APickupItemContainer& container, const FIntPoint& pos)
{
	check(false);
	//to do with int32
	if (auto coordinate{ containerMap.Find(&container) }) {
		//add validity check?
		//check if insertion is possible
		if (pos.X + (pos.Y) * xSize == *coordinate) {
			return 0;
		}

		FIntPoint size{ container.GetItemInfo()->GetInventorySize() };
		for (FIntPoint offset{ 0 }; offset.Y < size.Y; ++offset.Y) {
			for (offset.X = 0; offset.X < size.X; ++offset.X) {
				//int32 cellToCheck{ *coordinate + offset.X + (offset.Y) * xSize };
				if (int32 cellToCheck{ *coordinate + offset.X + (offset.Y) * xSize };
					cells[cellToCheck] != nullptr && cells[cellToCheck] != &container) {
					return 0;
				}
			}
		}
		for (FIntPoint offset{ 0 }; offset.Y < size.Y; ++offset.Y) {
			for (offset.X = 0; offset.X < size.X; ++offset.X) {
				cells[pos.X + offset.X + (offset.Y + pos.Y) * xSize] = &container;
			}
		}


		for (FIntPoint offset{ 0 }; offset.Y < size.Y; ++offset.Y) {
			for (offset.X = 0; offset.X < size.X; ++offset.X) {
				cells[*coordinate + offset.X + (offset.Y) * xSize] = nullptr;
			}
		}
		return container.GetItemsCount();
	}
	return false;

}

APickupItemContainer* UInventoryComponent::RemoveContainer(const FIntPoint& pos, bool bEject, bool bBroadcast)
{
	APickupItemContainer* container{ cells[pos.X + (pos.Y) * xSize] };
	if (RemoveContainer(*container, bBroadcast, bEject)) {
		return container;
	}
	else {
		return nullptr;
	}	
}

bool UInventoryComponent::RemoveContainer(APickupItemContainer& container, bool bEject, bool bBroadcast)
{
	int32 oldPos{ -999 };
	containerMap.RemoveAndCopyValue(&container, oldPos);
	if (oldPos < 0) {
		//UE_LOG(LogTemp, Error, TEXT("Removing container has wrong coordinates: X is %d, Y is %d"), containerPos.Key, containerPos.Value);
		return false;
	}
	if (container.RemoveFromInventory(bEject)) {
		FIntPoint size{ container.GetItemInfo()->GetInventorySize() };
		for (int32 y{ 0 }; y < size.Y; ++y) {
			for (int32 x{ 0 }; x < size.X; ++x) {
				cells[oldPos + x + y * xSize] = nullptr;
			}
		}
		if (bBroadcast && onInventoryUpdated.IsBound())
			onInventoryUpdated.Broadcast();
		return true;
	}
	else {
		return false;
	}
}

TMap<APickupItemContainer*, FIntPoint> UInventoryComponent::GetContainersInfo() const
{
	TMap<APickupItemContainer*, FIntPoint> out;
	for (auto& containerInfo : containerMap) {
		out.Add(containerInfo.Key, 
			{ containerInfo.Value % xSize, containerInfo.Value / xSize });
	}
	return out;
}

int32 UInventoryComponent::CheckItems(UPickupItemInfoBase& itemToFind, int32 requestedQuantity) const
{
	if (requestedQuantity < 1)
		return 0;
	int32 lackingQuantity{ requestedQuantity };
	auto iterator{ containerMap.CreateConstIterator() };
	while (lackingQuantity > 0 && iterator) {
		if (iterator.Key()->GetItemInfo() == &itemToFind) {
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

bool UInventoryComponent::TryUseItems(UPickupItemInfoBase& itemToUse, int32 quantityToUse, bool bBroadcast)
{
	if (CheckItems(itemToUse, quantityToUse) == quantityToUse) {
		auto iterator{ containerMap.CreateConstIterator() };
		while (quantityToUse > 0 && iterator) {
			if (iterator.Key()->GetItemInfo() == &itemToUse) {

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

bool UInventoryComponent::GetContainerCoordinates(APickupItemContainer& container, FIntPoint& outpos) const
{
	if (auto coordinate{ containerMap.Find(&container) }) {
		outpos.X = *coordinate % xSize;
		outpos.Y = *coordinate / xSize;
		return true;
	}
	else
		return false;
}

int32 UInventoryComponent::GetXSize() const noexcept
{
	return xSize;
}

int32 UInventoryComponent::GetYSize() const noexcept
{
	return ySize;
}


