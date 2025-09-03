// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "ProceduralMeshComponent.h"
#include "ViveSR.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_DualCameraImagePlane.generated.h"

//#define USE_UNDISTORT_MESH

UCLASS()
class AViveSR_DualCameraImagePlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AViveSR_DualCameraImagePlane();

	int distorted_img_plane_width;
	int distorted_img_plane_height;
	int undistorted_img_plane_width;
	int undistorted_img_plane_height;
	float camera_focal_length;
	int undistortedCx[2];
	int undistortedCy[2];

	void Initial();
	void SetMode(DualCameraDisplayMode mode);

	UProceduralMeshComponent* DistortedImagePlaneLeft;
	UProceduralMeshComponent* DistortedImagePlaneRight;
	UStaticMeshComponent* UndistortedImagePlaneLeft;
	UStaticMeshComponent* UndistortedImagePlaneRight;
	UStaticMeshComponent* DepthImagePlane;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FORCEINLINE void SetupImagePlaneMesh(UMeshComponent* Comp, FVector v= FVector(10000.0f, 0.0f, 0.0f))
	{
		if (!Comp) return;
		Comp->SetupAttachment(RootComponent);
		Comp->SetRelativeLocation(v);
		Comp->SetRelativeRotation(FRotator(0.0f, 90.0f, 90.0f));
		Comp->SetCollisionProfileName("NoCollision");
		Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Comp->bCastDynamicShadow = false;
		Comp->CastShadow = false;
		Comp->SetHiddenInGame(true);
	}
};
