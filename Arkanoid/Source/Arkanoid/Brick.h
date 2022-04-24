// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum BrickType
{
	Single		  	UMETA(DisplayName = "Single"),
	Double			UMETA(DisplayName = "Double"),
	None,
};

UCLASS()
class ARKANOID_API ABrick : public AActor
{
	GENERATED_BODY()
public:

	// Sets default values for this actor's properties
	ABrick();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* SM_Brick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* Box_Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<BrickType> Type;

	UPROPERTY(EditAnywhere)
		UMaterial* DoubleBrickMaterial;

	UPROPERTY(EditAnywhere)
		UMaterial* SingleBrickMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DoubleBlockProbability = 0.1;

	float SpeedModifierOnBounce = 1.01f;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexType, bool bFromSweep,
			const FHitResult& SweepResult);


	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			const FHitResult& Hit);

	void DestroyBrick();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void SetCurrentTypeMaterial();
};
