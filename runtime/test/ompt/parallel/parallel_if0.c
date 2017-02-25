// RUN: %libomp-compile-and-run | FileCheck %s
// REQUIRES: ompt
#include "callback.h"

int main()
{
//  print_frame(0);
  #pragma omp parallel if(0)
  {
//    print_frame(1);
    print_ids(0);
    print_ids(1);
//    print_frame(0);
    #pragma omp parallel if(0)
    {
//      print_frame(1);
      print_ids(0);
      print_ids(1);
      print_ids(2);
//      print_frame(0);
      #pragma omp task
      {
//        print_frame(1);
        print_ids(0);
        print_ids(1);
        print_ids(2);
        print_ids(3);
      }
    }
  }

  // Check if libomp supports the callbacks for this test.
  // CHECK-NOT: {{^}}0: Could not register callback 'ompt_callback_parallel_begin'
  // CHECK-NOT: {{^}}0: Could not register callback 'ompt_callback_parallel_end'
  // CHECK-NOT: {{^}}0: Could not register callback 'ompt_event_implicit_task_begin'
  // CHECK-NOT: {{^}}0: Could not register callback 'ompt_event_implicit_task_end'

  // CHECK: 0: NULL_POINTER=[[NULL:.*$]]
  // CHECK: {{^}}[[MASTER_ID:[0-9]+]]: ompt_event_parallel_begin: parent_task_id=[[PARENT_TASK_ID:[0-9]+]], parent_task_frame.exit=[[NULL]], parent_task_frame.reenter={{0x[0-f]+}}, parallel_id=[[PARALLEL_ID:[0-9]+]], requested_team_size=1, parallel_function=0x{{[0-f]+}}, invoker=[[PARALLEL_INVOKER:[0-9]+]]

  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID:[0-9]+]]
  // CHECK: {{^}}[[MASTER_ID]]: task level 0: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // CHECK: {{^}}[[MASTER_ID]]: task level 1: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame={{0x[0-f]+}}
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_parallel_begin: parent_task_id=[[IMPLICIT_TASK_ID]], parent_task_frame.exit={{0x[0-f]+}}, parent_task_frame.reenter={{0x[0-f]+}}, parallel_id=[[NESTED_PARALLEL_ID:[0-9]+]], requested_team_size=1, parallel_function=0x{{[0-f]+}}, invoker=[[PARALLEL_INVOKER]]
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID:[0-9]+]]
  // CHECK: {{^}}[[MASTER_ID]]: task level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // CHECK: {{^}}[[MASTER_ID]]: task level 1: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame={{0x[0-f]+}}
  // CHECK: {{^}}[[MASTER_ID]]: task level 2: parallel_id=0, task_id=[[PARENT_TASK_ID]], exit_frame=[[NULL]], reenter_frame={{0x[0-f]+}}

  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_task_create: parent_task_id=[[NESTED_IMPLICIT_TASK_ID]], parent_task_frame.exit={{0x[0-f]+}}, parent_task_frame.reenter={{0x[0-f]+}}, new_task_id=[[EXPLICIT_TASK_ID:[0-9]+]]
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_task_schedule: first_task_id=[[NESTED_IMPLICIT_TASK_ID]], second_task_id=[[EXPLICIT_TASK_ID]], prior_task_status=4
  // CHECK: {{^}}[[MASTER_ID]]: task level 0: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[EXPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame=[[NULL]]
  // CHECK: {{^}}[[MASTER_ID]]: task level 1: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame={{0x[0-f]+}}
  // CHECK: {{^}}[[MASTER_ID]]: task level 2: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], exit_frame={{0x[0-f]+}}, reenter_frame={{0x[0-f]+}}
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_task_schedule: first_task_id=[[EXPLICIT_TASK_ID]], second_task_id=[[NESTED_IMPLICIT_TASK_ID]], prior_task_status=1
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_task_end: task_id=[[EXPLICIT_TASK_ID]]


  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[NESTED_IMPLICIT_TASK_ID]]
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_parallel_end: parallel_id=[[NESTED_PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_end: parallel_id=[[PARALLEL_ID]], task_id=[[IMPLICIT_TASK_ID]]
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_parallel_end: parallel_id=[[PARALLEL_ID]], task_id=[[PARENT_TASK_ID]], invoker=[[PARALLEL_INVOKER]]

  return 0;
}