// Copyright saberuster. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TBSGridSystemTypes.generated.h"

UENUM()
enum class EGridPartType: uint8
{
    Face,
    Edge,
    Vertex
};

USTRUCT(BlueprintType)
struct FGridUnitCoordinate
{
    GENERATED_BODY()

    static const int32 InvalidPos = -1;
    
    FGridUnitCoordinate():X(InvalidPos),Y(InvalidPos){}
    FGridUnitCoordinate(int32 InX, int32 InY):X(InX),Y(InY){}

    UPROPERTY(BlueprintReadOnly)
    int32 X;

    UPROPERTY(BlueprintReadOnly)
    int32 Y;

    bool IsValid() const { return X > InvalidPos && Y > InvalidPos; }

    friend bool operator==(const FGridUnitCoordinate& Left, const FGridUnitCoordinate& Right)
    {
        return Left.IsValid() && Right.IsValid() && Left.X == Right.X && Left.Y == Right.Y;
    }

    friend bool operator!=(const FGridUnitCoordinate& Left, const FGridUnitCoordinate& Right)
    {
        return !Left.IsValid() || !Right.IsValid() || Left.X != Right.X || Left.Y != Right.Y;
    }

    friend uint32 GetTypeHash(const FGridUnitCoordinate& Coordinate)
    {
        return HashCombine(GetTypeHash(Coordinate.X), GetTypeHash(Coordinate.Y));
    }

    static float DistSquared(const FGridUnitCoordinate& C1, const FGridUnitCoordinate& C2)
    {
        return FVector::DistSquared2D(FVector(C1.X,C1.Y, 0.f), FVector(C2.X,C2.Y,0.f));
    }

    static float Distance(const FGridUnitCoordinate& C1, const FGridUnitCoordinate& C2)
    {
        return FVector::Dist2D(FVector(C1.X,C1.Y, 0.f), FVector(C2.X,C2.Y,0.f));
    }

    static FGridUnitCoordinate Offset(const FGridUnitCoordinate& Origin,int32 OffsetX, int32 OffsetY)
    {
        return FGridUnitCoordinate(Origin.X + OffsetX, Origin.Y + OffsetY);
    }

    static void Neighbors(const FGridUnitCoordinate& Origin, TArray<FGridUnitCoordinate>& OutCoordinates)
    {
        if (Origin.IsValid())
        {
            OutCoordinates.Reset(4);
            OutCoordinates.Add(Offset(Origin, 1, 0));
            OutCoordinates.Add(Offset(Origin, -1, 0));
            OutCoordinates.Add(Offset(Origin, 0, 1));
            OutCoordinates.Add(Offset(Origin, 0, -1));
        }
    }
};







