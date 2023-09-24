// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitchCodeOnly.h"
#include "FPSMod1.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"

// Sets default values
ALightSwitchCodeOnly::ALightSwitchCodeOnly()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    DesiredIntensity = 3000.0f;

    PointLight1 = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight1"));
    PointLight1->Intensity = DesiredIntensity;
    PointLight1->SetVisibility(true);  // bVisible does not work. Needed to use SetVisibility(true)
    RootComponent = PointLight1;

    Sphere1 = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere1"));
    Sphere1->InitSphereRadius(350.0f);
    Sphere1->SetCollisionProfileName(TEXT("Trigger"));
    Sphere1->SetupAttachment(RootComponent);  // Attaches sphere component to LightComponent1

    Sphere1->OnComponentBeginOverlap.AddDynamic(this, &ALightSwitchCodeOnly::OnOverlapBegin);       // set up a notification for when this component overlaps something
    Sphere1->OnComponentEndOverlap.AddDynamic(this, &ALightSwitchCodeOnly::OnOverlapEnd);       // set up a notification for when this component overlaps something

}

// Called when the game starts or when spawned
void ALightSwitchCodeOnly::BeginPlay()
{
    Super::BeginPlay();

    //DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 50, FColor::Green, true, -1, 0, 2);
}

// Called every frame
void ALightSwitchCodeOnly::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ALightSwitchCodeOnly::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        ToggleLight();
    }
}

void ALightSwitchCodeOnly::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        ToggleLight();
    }
}

void ALightSwitchCodeOnly::ToggleLight()
{
    PointLight1->ToggleVisibility();
}

