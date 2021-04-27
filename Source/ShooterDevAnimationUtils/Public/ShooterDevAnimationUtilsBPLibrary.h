// Copyright Bryan Robertson, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShooterDevAnimationUtilsBPLibrary.generated.h"

/* 
* UShooterDevAnimationUtilsBPLibrary
* 
* Blueprint library containing some useful helper functions for implementing animation modifiers
*/
UCLASS()
class UShooterDevAnimationUtilsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/** Get component space bone transform for a frame in an Animation Sequence */
	UFUNCTION(BlueprintCallable, Category = "ShooterDevAnimationBlueprintLibrary|Pose")
		static void GetComponentSpaceBoneTransformForFrame(class UAnimSequence* AnimationSequence, const FName BoneName, const int32 Frame, FTransform& ComponentSpaceBoneTransformOut);

	/** Retrieves Bone Pose data for the entire animation, for the given Bone Name in the Animation Sequence */
	UFUNCTION(BlueprintCallable, Category = "ShooterDevAnimationBlueprintLibrary|Pose")
		static void GetComponentSpaceBonePosesForAnimation(class UAnimSequence* AnimationSequence, FName BoneName, TArray<float>& Times, TArray<FTransform>& ComponentSpaceBonePosesOut);

	/** Retrieves Bone Pose data for the entire animation, for the given Bone Name in the Animation Sequence */
	UFUNCTION(BlueprintCallable, Category = "ShooterDevAnimationBlueprintLibrary|Pose")
		static void StoreRawKeyFrames(class UAnimSequence* AnimationSequence, FName BoneName, TArray<float>& Times, const TArray<FTransform>& ComponentSpaceBonePoses);
};
