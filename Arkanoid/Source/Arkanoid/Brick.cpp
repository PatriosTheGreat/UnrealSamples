// Fill out your copyright notice in the De	ription page of Project Settings.


#include "Brick.h"

#include "Logging/LogMacros.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Ball.h"
#include "ArkanoidGameModeBase.h"
#include "ArkanoidGameState.h"

#include <cstdlib>
#include <iostream>
#include <ctime>

// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM_Brick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brick"));

	SM_Brick->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Box_Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Box_Collision->SetBoxExtent(FVector(25.0f, 10.0f, 10.0f));

	RootComponent = Box_Collision;
}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("Brick %s"), *this->GetName());
	// Box_Collision->OnComponentBeginOverlap.AddDynamic(this, &ABrick::OnOverlapBegin);
	Box_Collision->OnComponentHit.AddDynamic(this, &ABrick::OnHit);
	float type_val = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	if (type_val < DoubleBlockProbability) {
		Type = Double;
	}
	else {
		Type = Single;
	}

	SetCurrentTypeMaterial();
}

// Called every frame
void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrick::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
				   FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Ball")) {
		ABall* MyBall = Cast<ABall>(OtherActor);

		FVector BallVelocity = MyBall->GetVelocity();
		BallVelocity *= (SpeedModifierOnBounce - 1.0f);

		MyBall->GetBall()->SetPhysicsLinearVelocity(BallVelocity, true);

		FTimerHandle UnusedHandle;
		if (Type == BrickType::Single) {
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABrick::DestroyBrick, 0.1f, false);
		}
		else if (Type == BrickType::Double) {
			Type = BrickType::Single;
			SetCurrentTypeMaterial();
		}

		AArkanoidGameState* gameState = (AArkanoidGameState*)GetWorld()->GetGameState();
		gameState->IncrementScore(20);
	}
}

void ABrick::SetCurrentTypeMaterial() {
	TArray<UStaticMeshComponent*> Components;
	GetComponents<UStaticMeshComponent>(Components);
	for (int i = 0; i < Components.Num(); i++)
	{
		UStaticMeshComponent* staticMeshComponent = Components[i];
		
		UMaterial* material = nullptr;
		if (Type == BrickType::Single) {
			material = SingleBrickMaterial;
		}
		else if (Type == BrickType::Double) {
			material = DoubleBrickMaterial;
		}

		staticMeshComponent->SetMaterial(0, material);
	}
}

void ABrick::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, 
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndexType, bool bFromSweep, 
	const FHitResult & SweepResult)
{
	if (OtherActor->ActorHasTag("Ball")) {
		ABall* MyBall = Cast<ABall>(OtherActor);

		FVector BallVelocity = MyBall->GetVelocity();
		BallVelocity *= (SpeedModifierOnBounce - 1.0f);

		MyBall->GetBall()->SetPhysicsLinearVelocity(BallVelocity, true);

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABrick::DestroyBrick, 0.1f, false);
	}
}

void ABrick::DestroyBrick()
{
	this->Destroy();
}

