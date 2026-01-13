// Fill out your copyright notice in the Description page of Project Settings.


#include "DataGameInstanceSubsystem.h"
#include "JsonUtilities.h"

void UDataGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	HTTPModule = &FHttpModule::Get();
}

void UDataGameInstanceSubsystem::Deinitialize()
{
}

void UDataGameInstanceSubsystem::Login()
{
	auto Request = HTTPModule->CreateRequest();

	//FString JSONData = TEXT("{\"model\": \"qwen3-vl:30b\", \"prompt\": \"언리언 엔진은 뭐야?\", \"stream\" : false}");
	FString JSONData = TEXT("{\"model\": \"gemma3:4b\", \"prompt\": \"언리언 엔진은 뭐야?\", \"stream\" : false}");

	UE_LOG(LogTemp, Warning, TEXT("JSONString : %s"), *JSONData);


	Request->SetURL("http://192.168.0.100:11434/api/generate");
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JSONData);
	Request->OnProcessRequestComplete().BindUObject(this, &UDataGameInstanceSubsystem::OnProcessLLMRequestComplete);
	Request->ProcessRequest();
}

void UDataGameInstanceSubsystem::OnProcessLLMRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		//호출 실패
		return;
	}

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("Code : %d, %s"), StatusCode, *(ResponseContent));
}


void UDataGameInstanceSubsystem::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessedSuccessfully)
{
	if (!bProcessedSuccessfully || !Response.IsValid())
	{
		//호출 실패
		return;
	}
	//호출 됐으나 서버에서 온 결과 처리

	int32 StatusCode = Response->GetResponseCode();
	FString ResponseContent = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("Code : %d, %s"), StatusCode, *(ResponseContent));

	//JSON String to Struct
	FLoginData LoginData;

	if (FJsonObjectConverter::JsonObjectStringToUStruct<FLoginData>(ResponseContent, &LoginData, 0, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("struct name  : %s, result : %d"), *LoginData.name, LoginData.result);
	}


	//일일이 변경
	auto JsonReader = TJsonReaderFactory<TCHAR>::Create(ResponseContent);

	TSharedPtr<FJsonObject> JsonObject;
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	auto Name = JsonObject->GetField(TEXT("name"), EJson::String);
	auto Result = JsonObject->GetField(TEXT("result"), EJson::Boolean);

	UE_LOG(LogTemp, Warning, TEXT("name  : %s, result : %d"), *Name->AsString(), Result->AsBool());
}

