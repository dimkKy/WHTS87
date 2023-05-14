// by Dmitry Kolontay


#include "CoreSystems/StairwellManager.h"
#include "Environment/Stairwell.h"
#include "Hash/CityHash.h"
#include "EngineUtils.h"
//#include "WHTS87Utils.h"
#include <iterator>

/*const FIntPoint UStairwellManager::SWTypeThresholds[4] = {
	{5, 7},
	{5, 7},
	{5, 7},
};*/


constexpr bool UStairwellManager::checkSWTypeThresholds(SIZE_T i)
{
	return (i >= std::size(SWTypeChances)) ||
		(SWTypeThresholds[i].first < SWTypeThresholds[i].second && 
			checkSWTypeThresholds(i + 1));
}

constexpr bool UStairwellManager::checkSWTypeChances(SIZE_T i)
{
	return (i >= std::size(SWTypeChances) - 1) ||
		(SWTypeChances[i] < SWTypeChances[i + 1] && checkSWTypeChances(i + 1));
}

UStairwellManager::UStairwellManager() :
	worldSeed{0}
{
	static_assert(checkSWTypeChances(), 
		"SWTypeChances formed badly : values should ascend");
	static_assert(checkSWTypeThresholds(),
		"checkSWTypeThresholds formed badly : x.first should be less then x.second");
	stairwells.Reserve(2 * SWTypeThresholds[
		static_cast<SIZE_T>(EStairwellType::RectangularWithTravel)].first);
}

bool UStairwellManager::ShouldCreateSubsystem(UObject* Outer) const
{
	return this->GetClass()->IsInBlueprint();
}

void UStairwellManager::SetSeed()
{
	SetSeed(FMath::RandHelper(TNumericLimits<int32>::Max()));
}

void UStairwellManager::SetSeed(int32 newSeed)
{
	worldSeed = newSeed;
}

void UStairwellManager::Init()
{
	UWorld* world = GetWorld();
	check(IsValid(world));
	/*GetAllStairwells(stairwells);
	if (stairwells.Num() != 0) {
		AddDefaultStairwell(2);
	}
	else {
		//do smthng idk
		check(false);
	}*/
}

void UStairwellManager::OnGameplayStart()
{
	AddStairwellsChunk();
}

void UStairwellManager::AddStairwellsChunk()
{
	auto type{ AddStairwell() };
	while (type != EStairwellType::RectangularWithTravel) {
		type = AddStairwell();
	}
	//chunkBorders.Add(stairwells.Num() - 1);
}

UE_NODISCARD AStairwell* UStairwellManager::ConstructStairwell(FTransform newSWTransform, EStairwellType type, int32 seed)
{
	//stairwells.Reserve
	auto* newStairwell{ GetWorld()->SpawnActorDeferred<AStairwell>(AStairwell::StaticClass(),
		newSWTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };

	//to construct body?
	//newStairwell->GenerateEntranceLadder(defaultLadderMesh);

	//choose main floor mesh
	ConstructStarweillBody(*newStairwell, type);

	

	/*iterate through sockets

	TDoubleLinkedList<UStaticMeshComponent*> componentsToLook;
	//TDoubleLinkedList<AActor*> componentsToIterate;
	for (auto const& socketName : newStairwell->mainFloor->GetAllSocketNames()) {
		//fist look up for sockets with a unique name
		if (socketName == socketNamingRules.toNextSWSocketName) {
			//UStaticMeshComponent* addedLadder{ newStairwell->AddJoint(defaultLadderMesh, socketName, newStairwell->mainFloor) };
			UStaticMeshComponent* addedLadder{ AddMeshComponent(defaultLadderMesh, socketName, newStairwell->mainFloor) };
			check(IsValid(addedLadder));
			componentsToLook.AddTail(addedLadder);
			if (type != EStairwellType::RectangularWithTravel && addedLadder->DoesSocketExist(socketNamingRules.toNextSWSocketName)) {
				//componentsToLook.AddTail(newStairwell->AddJoint(defaultRoundedSWMesh, socketNamingRules.toNextSWSocketName, addedLadder));
				componentsToLook.AddTail(AddMeshComponent(defaultRoundedSWMesh, socketNamingRules.toNextSWSocketName, addedLadder));
			}
			continue;
		}

		//socket1 socket2 etc
		auto socketNameString{ socketName.ToString() };
		if (socketNameString.Contains(socketNamingRules.doorFrameSocketName)) {
			//to place door or not
			if (seed % 2 == 0) {
				ADoor* newDoor{ GetWorld()->SpawnActorDeferred<ADoor>(ADoor::StaticClass(), newStairwell->mainFloor->GetSocketTransform(socketName),
					nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn) };
				newDoor->SetMeshes(defaultDoorFrameMesh, defaultDoorMesh);
				newDoor->FinishSpawning(newStairwell->mainFloor->GetSocketTransform(socketName), true);

				TInlineComponentArray<UStaticMeshComponent*> meshComponents;
				newDoor->GetComponents(meshComponents);
				for (auto& meshComponent : meshComponents) {
					componentsToLook.AddTail(CastChecked<UStaticMeshComponent>(meshComponent));
				}
				//auto newDoor = GetWorld()->SpawnActorDeferred<AStairwell>(AStairwell::StaticClass(), stairwell->mainFloor->GetSocketTransform(socketName), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			}
			else {
				//wall instead of door. Procedural mesh?
			}
			continue;
		}


		if (socketNameString.Contains(socketNamingRules.specialRoomSocketName)) {
			//add special room
			check(type != EStairwellType::CanonicalRounded);
			if (type == EStairwellType::RectangularSafe || type == EStairwellType::RectangularWithTravel
				|| (type == EStairwellType::RectangularDefault && seed % 3 == 0)) {
				componentsToLook.AddTail(newStairwell->AddJoint(defaulSpecialSWMesh, socketName, newStairwell->mainFloor));
			}
			else {
				//wall instead of door. Procedural mesh?
			}

			continue;
		}

	}

	newStairwell->FinishSpawning(newSWTransform); */

		//ParseMeshComponents(componentsToLook, type, seed, true);
		return newStairwell;
}


void UStairwellManager::AddDefaultStairwell(int32 quantity)
{
	for (int32 i{ 0 }; i < quantity; ++i) {
		stairwells.Add(ConstructStairwell( GetNewSWTransform(), 
			EStairwellType::CanonicalRounded, GetStairwellSeed(stairwells.Num() + 1)));
	}
}

EStairwellType UStairwellManager::AddStairwell()
{
	check(IsValid(GetWorld()) && stairwells.Num() > 0);

	int32 seed{ GetStairwellSeed(stairwells.Num()) };
	auto type{ GetStairwellType(seed) };
	stairwells.Add(ConstructStairwell(GetNewSWTransform(), type, seed));
	if (type == EStairwellType::RectangularWithTravel) {
		chunkBorders.Add(stairwells.Num() - 1);
	}
	return type;
}

void UStairwellManager::GetAllStairwells(TArray<AStairwell*>& out)
{
	out.Reset();
	//TSubclassOf<AStairwell> stairwellClass{ AStairwell::StaticClass() };
	for (TActorIterator<AActor> it{ GetWorld(), AStairwell::StaticClass() }; it; ++it)
	{
		out.Add(Cast<AStairwell>(*it));
	}

	//chunk borders?
}

int32 UStairwellManager::GetStairwellSeed(int32 stairwellNumber)
{
	char buffer[sizeof(stairwellNumber)];
	_itoa_s(stairwellNumber, buffer, 10);
	return CityHash64WithSeed(buffer, sizeof(stairwellNumber), worldSeed);
}

EStairwellType UStairwellManager::GetStairwellType(int32 seed)
{
	for (SIZE_T i = 0; i < static_cast<SIZE_T>(EStairwellType::EStairwellType_MAX); ++i) {
		if (SWTypeThresholds[i].second < stairwells.Num() - lastSWTypeNumber[i]) {
			return static_cast<EStairwellType>(i);
		}
	}

	seed /= SWTypeChances[std::size(SWTypeChances) - 1];

	for (SIZE_T i = 0; i < static_cast<SIZE_T>(EStairwellType::EStairwellType_MAX); ++i) {
		if (worldSeed < SWTypeChances[i] && 
			SWTypeThresholds[i].first > stairwells.Num() - lastSWTypeNumber[i]) {
			return static_cast<EStairwellType>(i);
		}
	
	}
	return static_cast<EStairwellType>(static_cast<int32>(EStairwellType::EStairwellType_MAX) - 1);
}

void UStairwellManager::ConstructStarweillBody(AStairwell& stairwell, EStairwellType type)
{
	if (type == EStairwellType::CanonicalRounded) {
		//stairwell.mainFloor->SetStaticMesh(defaultRoundedSWMesh);
	}
	else {
		//stairwell.mainFloor->SetStaticMesh(defaultRectangularSWMesh);
	}
}

FTransform UStairwellManager::GetNewSWTransform()
{
	if (stairwells.Num() - 1 == chunkBorders.Last()) {
		check(false);
		//TODO
		return FTransform();
	}
	else {
		return stairwells.Last()->GetNextSWSpawnPoint();
	}
}


