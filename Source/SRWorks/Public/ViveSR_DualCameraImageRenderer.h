// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ViveSR_UEnums.h"
#include "ViveSR_DualCameraImageRenderer.generated.h"

/**
* This is the actor can show PassThrough and depth image
*/

UCLASS()
class SRWORKS_API AViveSR_DualCameraImageRenderer : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AViveSR_DualCameraImageRenderer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVROrigin(AActor* vrOrigin) { vr_origin = vrOrigin; }
	void Initial();
	void Release();

    bool GPUMethod = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") DualCameraDisplayMode DisplayMode = DualCameraDisplayMode::MIX;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|DualCamera|ImageRenderer") bool PurpleFringeRemoval = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViveSR|DualCamera|ImageRenderer") bool UpdateDistortedMaterial = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|DualCamera|ImageRenderer") bool UpdateUndistortedMaterial = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViveSR|DualCamera|ImageRenderer") bool UpdateDepthMaterial = false;


    UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* DistortedLeftCameraImageMaterialInstance;
    UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* DistortedRightCameraImageMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* UndistortedLeftCameraImageMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* UndistortedRightCameraImageMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* DePurpleLeftMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* DePurpleRightMaterialInstance;
	UPROPERTY(EditAnywhere, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInterface* DepthMaterialInstance;

	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") TArray<UMaterialInstanceDynamic*> DistortedLeftCameraImageMaterials;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") TArray<UMaterialInstanceDynamic*> DistortedRightCameraImageMaterials;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") TArray<UMaterialInstanceDynamic*> UndistortedLeftCameraImageMaterials;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") TArray<UMaterialInstanceDynamic*> UndistortedRightCameraImageMaterials;

	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") TArray<UMaterialInstanceDynamic*> DepthMaterials;
    UMaterialInstanceDynamic *DistortedLeftCameraImageMaterial;
	UMaterialInstanceDynamic *DistortedRightCameraImageMaterial;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInstanceDynamic *UndistortedLeftCameraImageMaterial;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInstanceDynamic *UndistortedRightCameraImageMaterial;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInstanceDynamic *DePurpleLeftMaterial;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInstanceDynamic *DePurpleRightMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UMaterialInstanceDynamic *DepthDynamicMaterialSelf;
	// texture for UE4
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UTexture2D *ImageLeft;
	UPROPERTY(BlueprintReadOnly, Category = "ViveSR|DualCamera|ImageRenderer") UTexture2D *ImageRight;
	
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|ImageRenderer") UProceduralMeshComponent* GetDistortedImagePlane(DualCameraIndex eye);
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|ImageRenderer") UStaticMeshComponent* GetUndistortedImagePlane(DualCameraIndex eye);
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|ImageRenderer") UStaticMeshComponent* GetDepthImagePlane();
	UFUNCTION(BlueprintCallable, category = "ViveSR|DualCamera|ImageRenderer") void SetMode(DualCameraDisplayMode mode);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UArrowComponent* root_arrow;
	AActor* vr_origin = nullptr;
    AActor* image_plane = nullptr;

	int distorted_img_width;
	int distorted_img_height;
	int undistorted_img_width;
	int undistorted_img_height;

    int last_texture_update_time = 0;
	int last_depth_update_time = 0;

    float real_fps = 0.0f;
	float real_depth_fps = 0.0f;

	bool is_horus = false;
    bool need_depurple = false;
	DualCameraStatus camera_status = DualCameraStatus::IDLE;
    const int ori_img_idx = 0;
private:
	void UpdateUndistortedPose();
	void UpdateDistortedPose();
    void UpdatePassThroughImage(TArray<UMaterialInstanceDynamic*> matListLeft, TArray<UMaterialInstanceDynamic*> matListRight, bool de_purple);
    void UpdateDepthImage();
};
