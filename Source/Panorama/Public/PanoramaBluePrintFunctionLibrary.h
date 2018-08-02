/* Copyright 2011-2018 Dodgee Software All Rights Reserved. 
	Author: Shem Taylor */

#pragma once

// C/C++ Includes
#include <iostream>
#include <fstream>

// Unreal Engine Includes
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "Engine/World.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/TextureRenderTargetCube.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponentCube.h"
#include "Engine/SceneCaptureCube.h"
#include "Engine/Canvas.h"

// Third Party
#include "ThirdParty/libPNG/libPNG-1.5.27/png.h"
#include "ThirdParty/libJPG/jpge.h"
#include "libBMP.h"

// Boiler Plate Includes for Plugin
#include "PanoramaBluePrintFunctionLibrary.generated.h"

// Enumeration for descripting the different types of File Formats
UENUM(BlueprintType)
enum class EFileFormat : uint8
{
	EFF_BMP = 1 UMETA(DisplayName = "BMP"), // BITMAP Image Format
	//EFF_JPG = 2 UMETA(DisplayName = "JPG"), // JPEG Image Format
	//EFF_PNG = 3 UMETA(DisplayName = "PNG"), // PNG Image Format
	//EFF_TGA = 4 UMETA(DisplayName = "TGA"), // TGA Image Format
};

// Enumeration for describing the different type sof Panoramas
UENUM(BlueprintType)
enum class EPanoramaType : uint8
{
	EPT_EQIRECTANGULAR = 1 UMETA(DisplayName = "Equirectangular"),
	EPT_CUBEMAP = 2 UMETA(DisplayName = "CubeMap")
};

/* NOTES: 
	- When it comes to BluePrint Library functions, UPARAM treats const parameters as inputs
		and non-constant reference types are treated as outputs of the blueprint node */

/* The UPanoramaBluePrintFunctionLibrary implements a group of Usable BluePrint nodes */
UCLASS()
class PANORAMA_API UPanoramaBluePrintFunctionLibrary : public UBlueprintFunctionLibrary
{
	// ****************
	// * BOILER PLATE *
	// ****************

	/* This Macro adds creates our *.generated.h file
		when we generate project files. This occurs when you
		generate visual project files by for example right clicking
		on a uproject and selecting generate visual studio project files*/
	GENERATED_BODY()

	// ***********************
	// * RENDERING FUNCTIONS *
	// ***********************

	public:
		/* This function creates a blue print node that renders a Panorama to a file */
		UFUNCTION(BlueprintCallable, Category = "Rendering")
		static bool RenderPanorama(UPARAM(DisplayName = "Full File Path") FString& FullFilePath,
								   UPARAM(DisplayName = "Location") const FVector Location = FVector(0, 0, 0), 
								   UPARAM(DisplayName = "Filename") const FString FileName = "Ineni",
								   UPARAM(DisplayName = "File Format") const EFileFormat FileFormat = EFileFormat::EFF_BMP,
								   UPARAM(DisplayName = "Save Location") const FString SaveLocation = "C:/",
								   UPARAM(DisplayName = "Panorama Type") const EPanoramaType PanoramaType = EPanoramaType::EPT_EQIRECTANGULAR,
								   UPARAM(DisplayName = "Resolution X") const int ResolutionX = 640,
								   UPARAM(DisplayName = "Resolution Y") const int ResolutionY = 480);

	protected:
		/* This function will create a render target */
		static UTextureRenderTarget2D* CreateTextureRenderTarget2D(int32 Width=256, int32 Height=256, FLinearColor ClearColor = FLinearColor::White, float Gamma = 1);
		/* This function will create a render target cube */
		static UTextureRenderTargetCube* CreateTextureRenderTargetCube(int32 Side=256, bool bHDR=false, FLinearColor ClearColor = FLinearColor::White, float Gamma = 1);
		/* This function will save a UTextureRenderTarget to file with the filename you specify */
		static bool SaveTextureRenderTarget(UTextureRenderTarget* RenderTarget, FString FileName = "", EFileFormat FileFormat = EFileFormat::EFF_BMP);
};