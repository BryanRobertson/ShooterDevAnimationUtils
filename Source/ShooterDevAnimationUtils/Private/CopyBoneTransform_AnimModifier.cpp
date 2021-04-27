// Copyright Bryan Robertson, Inc. All Rights Reserved.

#include "CopyBoneTransform_AnimModifier.h"
#include "ShooterDevAnimationUtilsBPLibrary.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void UCopyBoneTransform_AnimModifier::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	// Cache existing track data for revert
	{
		const TArray<FName>& TrackNames = AnimationSequence->GetAnimationTrackNames();
		const int32 TrackIndex = TrackNames.IndexOfByKey(DestinationBoneName);

		if (TrackIndex != INDEX_NONE)
		{
			CachedPrevTrackData = AnimationSequence->GetRawAnimationTrack(TrackIndex);
		}
	}

	// Apply the animation modifier
	{
		TArray<float> Times;
		TArray<FTransform> ComponentSpaceSourceBoneTransforms;

		// Get source bone poses for the entire animation
		UShooterDevAnimationUtilsBPLibrary::GetComponentSpaceBonePosesForAnimation(AnimationSequence, SourceBoneName, Times, ComponentSpaceSourceBoneTransforms);

		// Copy to destination bone
		UShooterDevAnimationUtilsBPLibrary::StoreRawKeyFrames(AnimationSequence, DestinationBoneName, Times, ComponentSpaceSourceBoneTransforms);
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void UCopyBoneTransform_AnimModifier::OnRevert_Implementation(UAnimSequence* AnimationSequence)
{
	// Restore cached track data
	const TArray<FName>& TrackNames = AnimationSequence->GetAnimationTrackNames();
	const int32 TrackIndex = TrackNames.IndexOfByKey(DestinationBoneName);

	if (TrackIndex != INDEX_NONE)
	{
		FRawAnimSequenceTrack& AnimTrack = AnimationSequence->GetRawAnimationTrack(TrackIndex);
		AnimTrack = CachedPrevTrackData;
	}
}