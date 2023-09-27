#include "MyClass.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Runtime/Online/HTTP/Public/Http.h"

UMyClass::UMyClass()
{

}

void UMyClass::LoadImageFromURL(const FString& URL)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UMyClass::OnImageDownloaded);

    HttpRequest->SetURL(URL);
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("image/jpeg"));

    HttpRequest->ProcessRequest();
}

void UMyClass::OnImageDownloaded(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
    if (bSucceeded && HttpResponse.IsValid() && HttpResponse->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        TArray<uint8> ResponseData = HttpResponse->GetContent();

        IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
        TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);

        if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ResponseData.GetData(), ResponseData.Num()))
        {
            const TArray<uint8>* RawData = nullptr;
            if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
            {
                UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
                if (Texture)
                {
                    void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                    FMemory::Memcpy(TextureData, RawData->GetData(), RawData->Num());
                    Texture->PlatformData->Mips[0].BulkData.Unlock();
                    Texture->UpdateResource();

                    // TODO: use texture
                }
            }
        }
    }
}