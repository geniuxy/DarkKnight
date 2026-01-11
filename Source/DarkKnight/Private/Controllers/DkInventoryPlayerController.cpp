// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkInventoryPlayerController.h"

#include "DarkKnightDebugHelper.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DarkKnight/DarkKnight.h"
#include "Kismet/GameplayStatics.h"

ADkInventoryPlayerController::ADkInventoryPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADkInventoryPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItem();
}

void ADkInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		InputSubsystem->AddMappingContext(IMCInventory, 0);
	}
}

void ADkInventoryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::OnInteract);
}

void ADkInventoryPlayerController::OnInteract()
{
	Debug::Print(TEXT("我正在交互！"));
}

void ADkInventoryPlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector TraceForward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, TraceForward))
	{
		return;
	}

	const FVector TraceEnd = TraceStart + TraceForward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Item);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		Debug::Print(TEXT("开始定位到一个Item"));
	}

	if (LastActor.IsValid())
	{
		Debug::Print(TEXT("结束定位之前的Item"));
	}
}
