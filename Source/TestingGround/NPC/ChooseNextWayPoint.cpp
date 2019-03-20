#include "ChooseNextWayPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWayPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// GetPatrolPoints
	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) { return EBTNodeResult::Failed; }
	auto PatrolRoute = AIController->GetPawn()->FindComponentByClass<UPatrolRoute>();

	if(!ensure(PatrolRoute)){ return EBTNodeResult::Failed;}
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();

	if (PatrolPoints.Num() == 0) { 
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing control point."))
		return EBTNodeResult::Failed;
	}

	// Get to the next way point
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WayPoint.SelectedKeyName, PatrolPoints[Index]);

	// Cycle Index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
