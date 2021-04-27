// Copyright Bryan Robertson, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "CopyBoneTransform_AnimModifier.generated.h"

/**
 * Animation modifier that will copy bone transforms from one bone to another
 * My original use-case for this was to copy over transforms from the hand bone, to a weapon bone that was parented to a completely different part of the hierarchy
 */
UCLASS()
class UCopyBoneTransform_AnimModifier : public UAnimationModifier
{
	GENERATED_BODY()
	
	public:

		//=========================================================================================
		// Public Methods
		//=========================================================================================

		/** Apply the modifier for the given Animation Sequence */
		void OnApply_Implementation(UAnimSequence* AnimationSequence) override;

		/** Revert the modifier for the given Animation Sequence */
		void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

	private:

		//=========================================================================================
		// Private Properties
		//=========================================================================================

		/** Bone that we should copy transforms from*/
		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
			FName SourceBoneName;

		/** Bone that we should copy transforms to */
		UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
			FName DestinationBoneName;

		UPROPERTY()
			FRawAnimSequenceTrack CachedPrevTrackData;
};
