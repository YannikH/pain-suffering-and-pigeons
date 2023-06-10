// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
    SetRootComponent(VisualMesh);
    SphereComponent->SetGenerateOverlapEvents(true);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    VisualMesh->SetupAttachment(SphereComponent);


    this->velocity = FVector(
        -5 + FMath::RandRange(0,10),
        -5 + FMath::RandRange(0, 10),
        0.f
    );
}


// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FVector NewLocation = GetActorLocation() + this->velocity;
    
    TArray< AActor* > overlappingActors;
    SphereComponent->GetOverlappingActors(overlappingActors);
    FString debugText = FString::FromInt(overlappingActors.Num());
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, debugText);

    DrawDebugSphere(GetWorld(), SphereComponent->GetComponentLocation(), SphereComponent->GetScaledSphereRadius(), 10.f, FColor::Red, false);

    FRotator velocityRotation = this->velocity.Rotation();
    NewLocation.Z = 290;
    SetActorLocation(NewLocation);
    FRotator rotation = GetActorRotation();
    rotation.Yaw = velocityRotation.Yaw;
    rotation.Pitch = -90.f;
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, velocityRotation.ToString());

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, GetActorRotation().ToString());

    //SetActorRotation(rotation);
    //SetWorldRotation(rotation);
    VisualMesh->SetWorldRotation(rotation);

    //rotation.Pitch = 0;
    //SetActorRotation(rotation);
    //SetActorRelativeRotation(rotation);
}

