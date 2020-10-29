// Copyright saberuster. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TBSGridSystemTypes.h"
#include "GameFramework/Info.h"
#include "TBSGridSystemManager.generated.h"


class ATBSGridUnit;

struct PathFindNode 
{
    PathFindNode(){}

    PathFindNode(const FGridUnitCoordinate& InCoordinate):Coordinate(InCoordinate),Score(0){ Parent = nullptr; }

    PathFindNode(const FGridUnitCoordinate& InCoordinate, TSharedPtr<PathFindNode> InParent): Parent(InParent),Coordinate(InCoordinate),Score(0){}
    PathFindNode(const FGridUnitCoordinate& InCoordinate, TSharedPtr<PathFindNode> InParent, int32 InScore): Parent(InParent),Coordinate(InCoordinate),Score(InScore){}
    TSharedPtr<PathFindNode> Parent;

    FGridUnitCoordinate Coordinate;

    int32 Score;
};
/**
 * 
 */
UCLASS(Blueprintable)
class GRIDSYSTEM_API ATBSGridSystemManager : public AInfo
{
	GENERATED_BODY()

public:

    ATBSGridSystemManager();

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GridSystem")
    int32 GridSizeX;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GridSystem")
    int32 GridSizeY;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GridSystem")
    float GridExtentInCm;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GridSystem")
    TSubclassOf<ATBSGridUnit> DefaultGridPartClass;

#if WITH_EDITORONLY_DATA
private:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="GridSystem", meta=(AllowPrivateAccess="true"))
    bool bVisualized;
#endif

public:
    virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

    void RebuildGrids();

public:
    void CalculateCoordinateLocation(const FGridUnitCoordinate& Coordinate, FVector& OutLocation) const;
    void GridUnitNeighbors(FGridUnitCoordinate Coordinate, TArray<ATBSGridUnit*>& OutGridUnits);
    void ClearGrids();
    FGridUnitCoordinate FindGridUnitCoordinate(ATBSGridUnit* InGridUnit) const;

    UFUNCTION(BlueprintCallable, Category="GridSystem")
    void FindPath(ATBSGridUnit* Origin, ATBSGridUnit* Target, TArray<FGridUnitCoordinate>& OutPathCoordinates);

    UFUNCTION(BlueprintCallable, Category="GridSystem")
    void FindNeighbors(FGridUnitCoordinate Coordinate, TArray<FGridUnitCoordinate>& OutCoordinates);

    UFUNCTION(BlueprintCallable, Category="GridSystem")
    FGridUnitCoordinate FindSelfCoordinate(ATBSGridUnit* Origin);

    bool IsInGrid(const FGridUnitCoordinate& Coordinate);

    void GetCoordinateUnits(const TArray<FGridUnitCoordinate>& Coordinates, TArray<ATBSGridUnit*>& OutUnits);

    FORCEINLINE bool InGridRange(const FGridUnitCoordinate& Coordinate) const;

    void CoordinateNeighborsChecked(const FGridUnitCoordinate& Origin, TArray<FGridUnitCoordinate>& OutCoordinates);


private:

    UPROPERTY()
    TMap<FGridUnitCoordinate, ATBSGridUnit*> GridUnitArrays;

    UPROPERTY()
    bool bIsInitialized;
      
};
