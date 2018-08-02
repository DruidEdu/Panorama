#include "PanoramaBluePrintFunctionLibrary.h"

bool UPanoramaBluePrintFunctionLibrary::RenderPanorama(UPARAM(DisplayName = "Full File Path") FString& FullFilePath,
							UPARAM(DisplayName = "Location") const FVector Location, 
							UPARAM(DisplayName = "Filename") const FString FileName,
							UPARAM(DisplayName = "File Format") const EFileFormat FileFormat,
							UPARAM(DisplayName = "Save Location") const FString SaveLocation,
							UPARAM(DisplayName = "Panorama Type") const EPanoramaType PanoramaType,
							UPARAM(DisplayName = "Resolution X") const int ResolutionX,
							UPARAM(DisplayName = "Resolution Y") const int ResolutionY)
{
	// The result of this funtion
	bool FunctionResult = false;
	// Compute the FilePath
	FullFilePath = SaveLocation + FileName;

	// Send a message to the console
	UE_LOG(LogInit, Log, TEXT("RenderPanorama() called")); 

	// Branching Logic for the PanoramaType
	switch(PanoramaType)
	{
		case EPanoramaType::EPT_CUBEMAP:
		{
			// We make 6 textures one for each face of the Cube
			UTextureRenderTarget2D* PositiveXRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX, ResolutionY, FLinearColor::White, 1.0f);
			UTextureRenderTarget2D* NegativeXRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX, ResolutionY, FLinearColor::White, 1.0f);
			UTextureRenderTarget2D* PositiveYRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX, ResolutionY, FLinearColor::White, 1.0f);
			UTextureRenderTarget2D* NegativeYRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX, ResolutionY, FLinearColor::White, 1.0f);
			UTextureRenderTarget2D* PositiveZRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX, ResolutionY, FLinearColor::White, 1.0f);
			UTextureRenderTarget2D* NegativeZRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX, ResolutionY, FLinearColor::White, 1.0f);

			// Spawn a SceneCaptureComponent2D
			ASceneCapture2D* SceneCapture2DActor = GWorld->SpawnActor<ASceneCapture2D>(ASceneCapture2D::StaticClass());
			SceneCapture2DActor->GetCaptureComponent2D()->bCaptureEveryFrame = false;
			SceneCapture2DActor->GetCaptureComponent2D()->bCaptureOnMovement = false;
			// Position the SceneCaptureActor
			SceneCapture2DActor->SetActorLocation(Location);
			
			// Hide Pawns from Capture
			TArray<AActor*> Pawns;
			UGameplayStatics::GetAllActorsOfClass(SceneCapture2DActor, APawn::StaticClass(), Pawns);
			for(AActor* Pawn : Pawns)
			{
				SceneCapture2DActor->GetCaptureComponent2D()->HiddenActors.Add(Pawn);
			}				

			// CapturePositiveX
			SceneCapture2DActor->GetCaptureComponent2D()->TextureTarget = PositiveXRenderTarget2D;			
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(0.0f)));
			SceneCapture2DActor->GetCaptureComponent2D()->CaptureScene();
			
			// CaptureNegativeX
			SceneCapture2DActor->GetCaptureComponent2D()->TextureTarget = NegativeXRenderTarget2D;
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(180.0f)));
			SceneCapture2DActor->GetCaptureComponent2D()->CaptureScene();
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(-180.0f)));

			// CapturePositiveY
			SceneCapture2DActor->GetCaptureComponent2D()->TextureTarget = PositiveYRenderTarget2D;
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(90.0f)));
			SceneCapture2DActor->GetCaptureComponent2D()->CaptureScene();
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(-90.0f)));
			
			// CaptureNegativeY
			SceneCapture2DActor->GetCaptureComponent2D()->TextureTarget = NegativeYRenderTarget2D;
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(270.0f)));
			SceneCapture2DActor->GetCaptureComponent2D()->CaptureScene();
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(-270.0f)));
			
			// CapturePositiveZ
			SceneCapture2DActor->GetCaptureComponent2D()->TextureTarget = PositiveZRenderTarget2D;
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(90.0f)));
			SceneCapture2DActor->GetCaptureComponent2D()->CaptureScene();
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(-90.0f)));
			
			// CaptureNegativeZ
			SceneCapture2DActor->GetCaptureComponent2D()->TextureTarget = NegativeZRenderTarget2D;
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(180.0f)));
			SceneCapture2DActor->GetCaptureComponent2D()->CaptureScene();
			SceneCapture2DActor->SetActorRotation(FQuat(FVector(0.0f, 1.0f, 0.0f), FMath::DegreesToRadians(-180.0f)));

			// Composite the Captures into a final image
			// Create the Ultimate Texture which will be the cube map
			UTextureRenderTarget2D* FinalRenderTarget2D = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(ResolutionX * 6.0f, ResolutionY, FLinearColor::White, 1.0f);
			UCanvas* CompositingCanvas = 0;
			FDrawToRenderTargetContext CompositingCanvasRenderTargetContext;
			FVector2D CompositingCanvasSize;
			UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(SceneCapture2DActor, FinalRenderTarget2D, CompositingCanvas, CompositingCanvasSize, CompositingCanvasRenderTargetContext);
				CompositingCanvas->K2_DrawTexture(PositiveXRenderTarget2D, FVector2D(ResolutionX * 0.0f, 0.0f), FVector2D(ResolutionX, ResolutionY), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
				CompositingCanvas->K2_DrawTexture(NegativeXRenderTarget2D, FVector2D(ResolutionX * 1.0f, 0.0f), FVector2D(ResolutionX, ResolutionY), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
				CompositingCanvas->K2_DrawTexture(PositiveYRenderTarget2D, FVector2D(ResolutionX * 2.0f, 0.0f), FVector2D(ResolutionX, ResolutionY), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
				CompositingCanvas->K2_DrawTexture(NegativeYRenderTarget2D, FVector2D(ResolutionX * 3.0f, 0.0f), FVector2D(ResolutionX, ResolutionY), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
				CompositingCanvas->K2_DrawTexture(PositiveZRenderTarget2D, FVector2D(ResolutionX * 4.0f, 0.0f), FVector2D(ResolutionX, ResolutionY), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
				CompositingCanvas->K2_DrawTexture(NegativeZRenderTarget2D, FVector2D(ResolutionX * 5.0f, 0.0f), FVector2D(ResolutionX, ResolutionY), FVector2D(0.0f, 0.0f), FVector2D(1.0f, 1.0f));
			UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(0, CompositingCanvasRenderTargetContext);

			// Try and Save the FinalRenderTarget to Disk
			FunctionResult = UPanoramaBluePrintFunctionLibrary::SaveTextureRenderTarget(FinalRenderTarget2D, FullFilePath, FileFormat);			

			// Destroy the SceneCapture2DActor
			SceneCapture2DActor->Destroy();
			
			// Release the rendertargets
			FinalRenderTarget2D->ReleaseResource();
			FinalRenderTarget2D->ReleaseResource();
			NegativeZRenderTarget2D->ReleaseResource();
			PositiveZRenderTarget2D->ReleaseResource();
			NegativeYRenderTarget2D->ReleaseResource();
			PositiveYRenderTarget2D->ReleaseResource();
			NegativeXRenderTarget2D->ReleaseResource();
			PositiveXRenderTarget2D->ReleaseResource();

			break;
		}
		case EPanoramaType::EPT_EQIRECTANGULAR:
		{
			/* NOTE: The SceneCaptureCubeActor actually produces a Panorama as opposed to an actual 
				traditional style CubeMap. It is for that reason we use an ASceneCaptureCube here */

			/* TODO: I need to fix the EQIRECTANGULAR logic here, it does produce and image its just not correct 
				at the time of writing not sure why. This needs to be investigated */

			// Create a Cubic Render Target
			UTextureRenderTargetCube* CubicRenderTarget = UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTargetCube(FMath::Max(ResolutionX, ResolutionY), false, FLinearColor::White, 1.0f);
			// Spawn a SceneCaptureCubeActor
			ASceneCaptureCube* SceneCaptureCubeActor = GWorld->SpawnActor<ASceneCaptureCube>(ASceneCaptureCube::StaticClass());
				// Set the TextureTarget for the CaptureComponentCube (a component of the SceneCaptureCubeActor)
				SceneCaptureCubeActor->GetCaptureComponentCube()->TextureTarget = CubicRenderTarget;
				SceneCaptureCubeActor->GetCaptureComponentCube()->bCaptureEveryFrame = false;
				SceneCaptureCubeActor->GetCaptureComponentCube()->bCaptureOnMovement = false;
			// Position the SceneCaptureActor
			SceneCaptureCubeActor->SetActorLocation(Location);
			// Hide Pawns from Capture
			TArray<AActor*> Pawns;
			UGameplayStatics::GetAllActorsOfClass(SceneCaptureCubeActor, APawn::StaticClass(), Pawns);
			for(AActor* Pawn : Pawns)
			{
				SceneCaptureCubeActor->GetCaptureComponentCube()->HiddenActors.Add(Pawn);
			}				
			// Capture the Scene (render's to the CubeMap)
			SceneCaptureCubeActor->GetCaptureComponentCube()->CaptureScene();
			
			// Try and Save the CubicTexture to Disk
			FunctionResult = UPanoramaBluePrintFunctionLibrary::SaveTextureRenderTarget(CubicRenderTarget, FullFilePath, FileFormat);

			// Destroy the ScaneCaptureCubeActor
			SceneCaptureCubeActor->Destroy();
			// Release the CubicRender Target
			CubicRenderTarget->ReleaseResource(); 

			break;
		}
		default:
		{
			// Do Nothing

			break;
		}
	}
	// Return Function Result
	return FunctionResult;
}

UTextureRenderTarget2D* UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTarget2D(int32 Width, int32 Height, FLinearColor ClearColor, float Gamma)
{   
	UTextureRenderTarget2D* NewRenderTarget2D = NewObject<UTextureRenderTarget2D>();
	if(!NewRenderTarget2D)
	{
		return nullptr;
	} 
	NewRenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	NewRenderTarget2D->ClearColor = FLinearColor::White;
	NewRenderTarget2D->TargetGamma = Gamma; 
	NewRenderTarget2D->InitAutoFormat(Width, Height);
	return NewRenderTarget2D; 
}

UTextureRenderTargetCube* UPanoramaBluePrintFunctionLibrary::CreateTextureRenderTargetCube(int32 Side, bool bHDR, FLinearColor ClearColor, float Gamma)
{
	UTextureRenderTargetCube* NewRenderTargetCube = NewObject<UTextureRenderTargetCube>();
	if (!NewRenderTargetCube)
	{
		return nullptr;
	}
	NewRenderTargetCube->bHDR = bHDR;
	NewRenderTargetCube->ClearColor = FLinearColor::White;
	NewRenderTargetCube->TargetGamma = Gamma; 
	NewRenderTargetCube->InitAutoFormat(Side);
	return NewRenderTargetCube; 
}

bool UPanoramaBluePrintFunctionLibrary::SaveTextureRenderTarget(UTextureRenderTarget* RenderTarget, FString FileName, EFileFormat FileFormat)
{
	// Validate the Filename
	if (FileName.Len() == 0)
		return false;

	// Grab the RBGA Colour Values
	TArray<FColor> OutBMP;
	FRenderTarget *rt = RenderTarget->GameThread_GetRenderTargetResource();
	rt->ReadPixels(OutBMP);

	// Change the logic depending on the save format requestd
	switch(FileFormat)
	{
		case EFileFormat::EFF_BMP:
		{
			FileName.Append(FString(".bmp"));
			// Build the Image
			BmpImg img (RenderTarget->GetSurfaceWidth(), RenderTarget->GetSurfaceHeight());
			for (long y = 0; y < RenderTarget->GetSurfaceHeight(); y++)
			{
				for (long x = 0; x < RenderTarget->GetSurfaceWidth(); x++)
				{
					long index = x + y * RenderTarget->GetSurfaceWidth();
					FColor colour = OutBMP[index];
					img.set_pixel (x, y, colour.R, colour.G, colour.B);
				}
			}
			// Write the Image
			img.write(std::string(TCHAR_TO_UTF8(*FileName)));
			break;
		}
		//case EFileFormat::EFF_JPG:
		//{
		//	// TODO: implement me with libJPG
		//	break;
		//}
		//case EFileFormat::EFF_PNG:
		// {
		//	/* NOTE: I chose not to use the function UKismetRenderingLibrary::ExportRenderTarget(...) because it only works
		//		with UTextureRenderTarget2D Objects. The function can be forced to export PNG or EXR depending on the number 
		//		of bits used per colour. If the function took the base class of the Render targets that would make it preferable*/
		//	// TODO: implement me with libPNG
		//	break;
		//}
		default:
		{
			break;
		}
	}

	// Success
	return true;
}