// Copyright saberuster. All Rights Reserved.


#include "TBSGridSystemManager.h"
#include "TBSGridUnit.h"


ATBSGridSystemManager::ATBSGridSystemManager():
    GridSizeX(10),
    GridSizeY(10),
    GridExtentInCm(100)
{
#if WITH_EDITORONLY_DATA
    bVisualized = false;
#endif
}

void ATBSGridSystemManager::OnConstruction(const FTransform& Transform)
{
    //Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
    if (bVisualized && !bIsInitialized)
    {
        RebuildGrids();
    }
#endif

}

#if WITH_EDITOR

void ATBSGridSystemManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    FProperty* ChangedProperty = PropertyChangedEvent.Property;
    if (ChangedProperty && ChangedProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ATBSGridSystemManager, bVisualized))
    {
        if (bVisualized)
        {
            RebuildGrids();
        }else
        {
            ClearGrids();
        }
    }
}

#endif

void ATBSGridSystemManager::RebuildGrids()
{
    if (!DefaultGridPartClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DefaultGridPartClass is invalid!"));
        return;
    }

    ClearGrids();

    FVector CurrentLocation;

    for (int32 X = 0; X < GridSizeX; ++X)
    {
        for (int32 Y = 0; Y < GridSizeY; ++Y)
        {
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.Owner = this;
            ATBSGridUnit* NewActor = GetWorld()->SpawnActor<ATBSGridUnit>(DefaultGridPartClass, SpawnParameters);
            FGridUnitCoordinate CurrentCoordinate = FGridUnitCoordinate(X, Y);
            GridUnitArrays.Add(CurrentCoordinate, NewActor);
            CalculateCoordinateLocation(CurrentCoordinate,CurrentLocation);
            NewActor->SetActorLocation(CurrentLocation);
        }
    }

    bIsInitialized = true;
}

void ATBSGridSystemManager::CalculateCoordinateLocation(const FGridUnitCoordinate& Coordinate, FVector& OutLocation) const
{
    const FVector& OriginLocation = GetActorLocation();
    const FVector DeltaLocation = FVector(Coordinate.X * 1.f, Coordinate.Y * 1.f, 0.f) * 2 * GridExtentInCm;
    OutLocation = OriginLocation + DeltaLocation;
}

FGridUnitCoordinate ATBSGridSystemManager::FindGridUnitCoordinate(ATBSGridUnit* InGridUnit) const
{
    const FGridUnitCoordinate* Coordinate = GridUnitArrays.FindKey(InGridUnit);
    if (Coordinate)
    {
        return *Coordinate;
    }
    return FGridUnitCoordinate();
}
void ATBSGridSystemManager::FindPath(ATBSGridUnit* Origin, ATBSGridUnit* Target, TArray<FGridUnitCoordinate>& OutPathCoordinates)
{
    FGridUnitCoordinate OriginCoord = FindGridUnitCoordinate(Origin);
    FGridUnitCoordinate TargetCoord = FindGridUnitCoordinate(Target);
    if (!OriginCoord.IsValid() || !TargetCoord.IsValid())
    {
        return;
    }
    OutPathCoordinates.Reset();

    TArray<TSharedPtr<PathFindNode>> PriorityArray;
    TArray<TSharedPtr<PathFindNode>> ClosedArray;
    bool bEndFind = false;

    PriorityArray.Add(MakeShared<PathFindNode>(OriginCoord));

    while (!bEndFind && PriorityArray.Num() > 0)
    {
        TSharedPtr<PathFindNode> Node = PriorityArray.Pop();
        if (Node->Coordinate == TargetCoord)
        {
            bEndFind = true;
            OutPathCoordinates.Add(Node->Coordinate);
            while (Node->Parent.IsValid())
            {
                OutPathCoordinates.Add(Node->Parent->Coordinate);
                Node = Node->Parent;
            }
        }else
        {
            TSharedPtr<PathFindNode> CurrentCoord =  ClosedArray[ClosedArray.Add(Node)];
            TArray<FGridUnitCoordinate> NeighborsCoordinates;
            CoordinateNeighborsChecked(CurrentCoord->Coordinate, NeighborsCoordinates);

            for (const FGridUnitCoordinate& NeighborsCoordinate : NeighborsCoordinates)
            {
                if(!ClosedArray.FindByPredicate([&NeighborsCoordinate](TSharedPtr<PathFindNode> PathNode){return PathNode->Coordinate == NeighborsCoordinate;}))
                {   
                    PriorityArray.Add(MakeShared<PathFindNode>(NeighborsCoordinate, CurrentCoord, CurrentCoord->Score + 1));
                }
            }


            PriorityArray.Sort([](const TSharedPtr<PathFindNode> A,const TSharedPtr<PathFindNode> B){return B->Score < A->Score;});
            
            //PriorityArray.Sort();        
        }
    }

}

void ATBSGridSystemManager::FindNeighbors(FGridUnitCoordinate Coordinate,TArray<FGridUnitCoordinate>& OutCoordinates)
{
     FGridUnitCoordinate::Neighbors(Coordinate, OutCoordinates);
     OutCoordinates.RemoveAll([=](const FGridUnitCoordinate& RCoordinate){return RCoordinate.X < 0 || RCoordinate.X >= GridSizeX || RCoordinate.Y < 0 || RCoordinate.Y >= GridSizeY;});
}

FGridUnitCoordinate ATBSGridSystemManager::FindSelfCoordinate(ATBSGridUnit* Origin)
{
    return FindGridUnitCoordinate(Origin);
}

bool ATBSGridSystemManager::IsInGrid(const FGridUnitCoordinate& Coordinate)
{
    return Coordinate.IsValid() && Coordinate.X < GridSizeX && Coordinate.Y < GridSizeY;
}

void ATBSGridSystemManager::GetCoordinateUnits(const TArray<FGridUnitCoordinate>& Coordinates, TArray<ATBSGridUnit*>& OutUnits)
{
    if (Coordinates.Num() > 0)
    {
        OutUnits.Reset(Coordinates.Num());
        for (const FGridUnitCoordinate& Coordinate : Coordinates)
        {
            OutUnits.Add(GridUnitArrays.FindRef(Coordinate));
        }
    }
}

bool ATBSGridSystemManager::InGridRange(const FGridUnitCoordinate& Coordinate) const
{
      return Coordinate.X >= 0 && Coordinate.Y >= 0 && Coordinate.X < GridSizeX && Coordinate.Y < GridSizeY;
}

void ATBSGridSystemManager::CoordinateNeighborsChecked(const FGridUnitCoordinate& Origin, TArray<FGridUnitCoordinate>& OutCoordinates)
{
    FGridUnitCoordinate::Neighbors(Origin, OutCoordinates);
    OutCoordinates.RemoveAll([&](const FGridUnitCoordinate& Coordinate){return !InGridRange(Coordinate);});
}


void ATBSGridSystemManager::GridUnitNeighbors(FGridUnitCoordinate Coordinate, TArray<ATBSGridUnit*>& OutGridUnits)
{
    TArray<FGridUnitCoordinate> NeighborsCoordinates;
    FGridUnitCoordinate::Neighbors(Coordinate, NeighborsCoordinates);
    for (const FGridUnitCoordinate& NeighborsCoordinate : NeighborsCoordinates)
    {
        ATBSGridUnit* GridUnit = GridUnitArrays.FindRef(NeighborsCoordinate);
        OutGridUnits.Add(GridUnit);
    }
}

void ATBSGridSystemManager::ClearGrids()
{
    for(const auto& Elem : GridUnitArrays)
    {
        Elem.Value->Destroy();
    }

    GridUnitArrays.Reset();
    bIsInitialized = false;
}



