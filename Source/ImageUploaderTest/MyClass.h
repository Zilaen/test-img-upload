#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyClass.generated.h"

/**
 *
 */
UCLASS()
class IMAGEUPLOADERTEST_API UMyClass : public UObject
{
    GENERATED_BODY()

public:
    UMyClass();

    void LoadImageFromURL(const FString& URL);

protected:
    void OnImageDownloaded(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};