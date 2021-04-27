// Copyright Bryan Robertson, Inc. All Rights Reserved.

#include "ShooterDevAnimationUtilsBPLibrary.h"
#include "ShooterDevAnimationUtils.h"
#include "AnimationBlueprintLibrary.h"
#include "Animation/AnimSequence.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
UShooterDevAnimationUtilsBPLibrary::UShooterDevAnimationUtilsBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/*static*/ void UShooterDevAnimationUtilsBPLibrary::GetComponentSpaceBoneTransformForFrame(UAnimSequence* AnimationSequence, const FName BoneName, const int32 Frame, FTransform& TransformOut)
{
	if (AnimationSequence == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetComponentSpaceBoneTransformForFrame: Failed because AnimationSequence is null!"));
		return;
	}

	// Get path to root
	TArray<FName> BonePath;
	BonePath.Reserve(16);

	UAnimationBlueprintLibrary::FindBonePathToRoot(AnimationSequence, BoneName, BonePath);

	// Now apply bone transforms all the way down the chain to get the component-space transform
	TransformOut = FTransform::Identity;

	for (int32 index = 0, count = BonePath.Num(); index < count; ++index)
	{
		const FName& BoneAtIndex = BonePath[index];
		constexpr bool bShouldExtractRootMotion = false;

		FTransform BoneTransform;
		UAnimationBlueprintLibrary::GetBonePoseForFrame(AnimationSequence, BoneAtIndex, Frame, bShouldExtractRootMotion, BoneTransform);

		TransformOut *= BoneTransform;
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/*static*/ void UShooterDevAnimationUtilsBPLibrary::GetComponentSpaceBonePosesForAnimation(UAnimSequence* AnimationSequence, FName BoneName, TArray<float>& TimesOut, TArray<FTransform>& ComponentSpaceBonePosesOut)
{
	if (AnimationSequence == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetComponentSpaceBonePosesForAnimation: Failed because AnimationSequence is null!"));
		return;
	}

	const int32 FrameCount = AnimationSequence->GetRawNumberOfFrames();
	ComponentSpaceBonePosesOut.Reserve(FrameCount);
	TimesOut.Reserve(FrameCount);

	for (int32 frameIndex = 0; frameIndex < FrameCount; ++frameIndex)
	{
		FTransform& OutputTransform = ComponentSpaceBonePosesOut.AddDefaulted_GetRef();
		GetComponentSpaceBoneTransformForFrame(AnimationSequence, BoneName, frameIndex, OutputTransform);

		float& OutputTime = TimesOut.AddZeroed_GetRef();
		UAnimationBlueprintLibrary::GetTimeAtFrame(AnimationSequence, frameIndex, OutputTime);
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/*static*/ void UShooterDevAnimationUtilsBPLibrary::StoreRawKeyFrames(UAnimSequence* AnimationSequence, FName BoneName, TArray<float>& Times, const TArray<FTransform>& ComponentSpacePoseTransforms)
{
	if (AnimationSequence == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("StoreRawKeyFrames: Failed because AnimationSequence is null!"));
		return;
	}

	const USkeleton* Skeleton = AnimationSequence->GetSkeleton();
	FName ParentBoneName = NAME_None;

	// Get Parent bone name
	{
		const FReferenceSkeleton& RefSkeleton = AnimationSequence->GetSkeleton()->GetReferenceSkeleton();

		const int32 BoneIndex = RefSkeleton.FindRawBoneIndex(BoneName);
		const int32 ParentBoneIndex = RefSkeleton.GetRawParentIndex(BoneIndex);

		if (ParentBoneIndex != INDEX_NONE)
		{
			ParentBoneName = RefSkeleton.GetBoneName(ParentBoneIndex);
		}
	}

	const int32 FrameCount = ComponentSpacePoseTransforms.Num();

	// Output keyframes
	{
		// Reserve space for track data
		FRawAnimSequenceTrack TrackData;
		TrackData.PosKeys.Reserve(FrameCount);
		TrackData.RotKeys.Reserve(FrameCount);
		TrackData.ScaleKeys.Reserve(FrameCount);

		// Bone transform in bone space
		FTransform BoneTransformBS;

		// Parent bone transform in component space
		FTransform ParentBoneTransformCS;

		for (int32 frameIndex = 0; frameIndex < FrameCount; ++frameIndex)
		{
			// Get bone transform relative to parent bone
			if (ParentBoneName != NAME_None)
			{
				GetComponentSpaceBoneTransformForFrame(AnimationSequence, ParentBoneName, frameIndex, ParentBoneTransformCS);
				BoneTransformBS = ComponentSpacePoseTransforms[frameIndex] * ParentBoneTransformCS.Inverse();
			}
			else
			{
				BoneTransformBS = ComponentSpacePoseTransforms[frameIndex];
			}

			TrackData.PosKeys.Add(BoneTransformBS.GetTranslation());
			TrackData.RotKeys.Add(BoneTransformBS.GetRotation());
			TrackData.ScaleKeys.Add(BoneTransformBS.GetScale3D());
		}

		// Add the new track
		const int32 TrackIndex = AnimationSequence->AddNewRawTrack(BoneName, &TrackData);
	}
}