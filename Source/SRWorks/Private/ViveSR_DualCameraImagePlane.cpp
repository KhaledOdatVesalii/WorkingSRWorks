// Fill out your copyright notice in the Description page of Project Settings.

#include "ViveSR_DualCameraImagePlane.h"
#include "ViveSR_DualCameraImageCapture.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AViveSR_DualCameraImagePlane::AViveSR_DualCameraImagePlane()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

    FVector anchorLeft = FVector((10 - 0.071f), -0.0325f, 0.0026f);
    FVector anchorRight = FVector((10 - 0.071f), 0.0325f, 0.0026f);
    DistortedImagePlaneLeft = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMeshLeft"));
    SetupImagePlaneMesh(DistortedImagePlaneLeft, anchorLeft * 100);
    DistortedImagePlaneRight = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMeshRight"));
    SetupImagePlaneMesh(DistortedImagePlaneRight, anchorRight * 100);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneVisualAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    if (PlaneVisualAsset.Succeeded())
    {
        UndistortedImagePlaneLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Undistorted Left)"));
        UndistortedImagePlaneLeft->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(UndistortedImagePlaneLeft, anchorLeft * 100);
        UndistortedImagePlaneRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Undistorted Right)"));
        UndistortedImagePlaneRight->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(UndistortedImagePlaneRight, anchorRight * 100);

        DepthImagePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ImagePlane (Depth)"));
        DepthImagePlane->SetStaticMesh(PlaneVisualAsset.Object);
        SetupImagePlaneMesh(DepthImagePlane);
    }
    else
    {
        UE_LOG(LogViveSR, Warning, TEXT("Image plane mesh not found."));
    }
}

// Called when the game starts or when spawned
void AViveSR_DualCameraImagePlane::BeginPlay()
{
    Super::BeginPlay();

}

void AViveSR_DualCameraImagePlane::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}
// Called every frame
void AViveSR_DualCameraImagePlane::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AViveSR_DualCameraImagePlane::Initial()
{
    distorted_img_plane_width = ViveSR_DualCameraImageCapture::Instance()->distorted_img_width;
    distorted_img_plane_height = ViveSR_DualCameraImageCapture::Instance()->distorted_img_height;
    undistorted_img_plane_width = ViveSR_DualCameraImageCapture::Instance()->undistorted_img_width;
    undistorted_img_plane_height = ViveSR_DualCameraImageCapture::Instance()->undistorted_img_height;

    camera_focal_length = ViveSR_DualCameraImageCapture::Instance()->focal_length_left; //(float)(ViveSR_DualCameraImageCapture::Instance()->CameraParameters.focal_length_left);
    undistortedCx[(int)DualCameraIndex::LEFT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cx_left;
    undistortedCx[(int)DualCameraIndex::RIGHT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cx_right;
    undistortedCy[(int)DualCameraIndex::LEFT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cy_left;
    undistortedCy[(int)DualCameraIndex::RIGHT] = ViveSR_DualCameraImageCapture::Instance()->undistorted_cy_right;

    UndistortedImagePlaneLeft->SetRelativeScale3D(FVector(10 * (float)undistorted_img_plane_width / camera_focal_length, 10 * (float)undistorted_img_plane_height / camera_focal_length, 1.0f));
    UndistortedImagePlaneRight->SetRelativeScale3D(FVector(10 * (float)undistorted_img_plane_width / camera_focal_length, 10 * (float)undistorted_img_plane_height / camera_focal_length, 1.0f));

}

void AViveSR_DualCameraImagePlane::SetMode(DualCameraDisplayMode mode)
{
    switch (mode)
    {
    case DualCameraDisplayMode::MIX:
#ifdef USE_UNDISTORT_MESH
        DistortedImagePlaneLeft->SetHiddenInGame(false);
        DistortedImagePlaneRight->SetHiddenInGame(false);
#else
        UndistortedImagePlaneLeft->SetHiddenInGame(false);
        UndistortedImagePlaneRight->SetHiddenInGame(false);
#endif
        SetActorTickEnabled(true);
        break;
    case DualCameraDisplayMode::VIRTUAL:
    default:
#ifdef USE_UNDISTORT_MESH
        DistortedImagePlaneLeft->SetHiddenInGame(true);
        DistortedImagePlaneRight->SetHiddenInGame(true);
#else
        UndistortedImagePlaneLeft->SetHiddenInGame(true);
        UndistortedImagePlaneRight->SetHiddenInGame(true);
#endif
        SetActorTickEnabled(false);
        break;
    }
}
