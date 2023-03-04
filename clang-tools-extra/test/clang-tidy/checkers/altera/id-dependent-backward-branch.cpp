// RUN: %check_clang_tidy %s altera-id-dependent-backward-branch %t -- -header-filter=.* "--" -cl-std=CL1.2 -c

void error() {
  // ==== Conditional Expressions ====
  int accumulator = 0;
  for (int i = 0; i < get_local_id(0); i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to ID function call and may cause performance degradation [altera-id-dependent-backward-branch]
    accumulator++;
  }

  int j = 0;
  while (j < get_local_id(0)) {
    // CHECK-NOTES: :[[@LINE-1]]:10: warning: backward branch (while loop) is ID-dependent due to ID function call and may cause performance degradation [altera-id-dependent-backward-branch]
    accumulator++;
  }

  do {
    accumulator++;
  } while (j < get_local_id(0));
  // CHECK-NOTES: :[[@LINE-1]]:12: warning: backward branch (do loop) is ID-dependent due to ID function call and may cause performance degradation [altera-id-dependent-backward-branch]

  // ==== Assignments ====
  int ThreadID = get_local_id(0);

  while (j < ThreadID) {
    // CHECK-NOTES: :[[@LINE-1]]:10: warning: backward branch (while loop) is ID-dependent due to variable reference to 'ThreadID' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-4]]:3: note: assignment of ID-dependent variable ThreadID
    accumulator++;
  }

  do {
    accumulator++;
  } while (j < ThreadID);
  // CHECK-NOTES: :[[@LINE-1]]:12: warning: backward branch (do loop) is ID-dependent due to variable reference to 'ThreadID' and may cause performance degradation [altera-id-dependent-backward-branch]
  // CHECK-NOTES: :[[@LINE-12]]:3: note: assignment of ID-dependent variable ThreadID

  int ThreadIDAssigned = 0;
  ThreadIDAssigned = get_local_id(0) * 2;

  for (int i = 0; i < ThreadIDAssigned; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to variable reference to 'ThreadIDAssigned' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-4]]:3: note: assignment of ID-dependent variable ThreadIDAssigned
    accumulator++;
  }

  struct { int IDDepField; } Example;
  Example.IDDepField = get_local_id(0);

  for (int i = 0; i < Example.IDDepField; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to member reference to 'IDDepField' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-4]]:3: note: assignment of ID-dependent field IDDepField
    accumulator++;
  }

  while (j < Example.IDDepField) {
    // CHECK-NOTES: :[[@LINE-1]]:10: warning: backward branch (while loop) is ID-dependent due to member reference to 'IDDepField' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-10]]:3: note: assignment of ID-dependent field IDDepField
    accumulator++;
  }

  do {
    accumulator++;
  } while (j < Example.IDDepField);
  // CHECK-NOTES: :[[@LINE-1]]:12: warning: backward branch (do loop) is ID-dependent due to member reference to 'IDDepField' and may cause performance degradation [altera-id-dependent-backward-branch]
  // CHECK-NOTES: :[[@LINE-18]]:3: note: assignment of ID-dependent field IDDepField

  // ==== Inferred Assignments ====
  int ThreadIDVarFromVar = ThreadID * 2;
  for (int i = 0; i < ThreadIDVarFromVar; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to variable reference to 'ThreadIDVarFromVar' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-3]]:28: note: inferred assignment of ID-dependent value from ID-dependent variable ThreadID
    // CHECK-NOTES: :[[@LINE-49]]:3: note: assignment of ID-dependent variable ThreadID
    accumulator++;
  }

  int ThreadIDVarAssignFromVar;
  ThreadIDVarAssignFromVar = ThreadID * 2;
  for (int i = 0; i < ThreadIDVarAssignFromVar; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to variable reference to 'ThreadIDVarAssignFromVar' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-3]]:30: note: inferred assignment of ID-dependent value from ID-dependent variable ThreadID
    // CHECK-NOTES: :[[@LINE-58]]:3: note: assignment of ID-dependent variable ThreadID
    accumulator++;
  }

  int ThreadIDVarFromMember = Example.IDDepField * 2;
  for (int i = 0; i < ThreadIDVarFromMember; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to variable reference to 'ThreadIDVarFromMember' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-3]]:31: note: inferred assignment of ID-dependent value from ID-dependent member IDDepField
    // CHECK-NOTES: :[[@LINE-42]]:3: note: assignment of ID-dependent field IDDepField
    accumulator++;
  }

  int ThreadIDVarAssignFromMember;
  ThreadIDVarAssignFromMember = Example.IDDepField * 2;
  for (int i = 0; i < ThreadIDVarAssignFromMember; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to variable reference to 'ThreadIDVarAssignFromMember' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-3]]:33: note: inferred assignment of ID-dependent value from ID-dependent member IDDepField
    // CHECK-NOTES: :[[@LINE-51]]:3: note: assignment of ID-dependent field IDDepField
    accumulator++;
  }

  struct { int IDDepFieldFromVar; } ExampleFromVar;
  ExampleFromVar.IDDepFieldFromVar = ThreadID * 2;
  for (int i = 0; i < ExampleFromVar.IDDepFieldFromVar; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to member reference to 'IDDepFieldFromVar' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-3]]:38: note: inferred assignment of ID-dependent member from ID-dependent variable ThreadID
    // CHECK-NOTES: :[[@LINE-84]]:3: note: assignment of ID-dependent variable ThreadID
    accumulator++;
  }

  struct { int IDDepFieldFromMember; } ExampleFromMember;
  ExampleFromMember.IDDepFieldFromMember = Example.IDDepField * 2;
  for (int i = 0; i < ExampleFromMember.IDDepFieldFromMember; i++) {
    // CHECK-NOTES: :[[@LINE-1]]:19: warning: backward branch (for loop) is ID-dependent due to member reference to 'IDDepFieldFromMember' and may cause performance degradation [altera-id-dependent-backward-branch]
    // CHECK-NOTES: :[[@LINE-3]]:44: note: inferred assignment of ID-dependent member from ID-dependent member IDDepField
    // CHECK-NOTES: :[[@LINE-69]]:3: note: assignment of ID-dependent field IDDepField
    accumulator++;
  }

  // ==== Unused Inferred Assignments ====
  int UnusedThreadID = Example.IDDepField; // OK: not used in any loops

  struct { int IDDepField; } UnusedStruct;
  UnusedStruct.IDDepField = ThreadID * 2; // OK: not used in any loops
}

int foo(int);

void success() {
  int accumulator = 0;

  for (int i = 0; i < 1000; i++) {
    if (i < get_local_id(0)) {
      accumulator++;
    }
  }

  // ==== Conditional Expressions ====
  for (int i = 0; i < foo(0); i++) {
    accumulator++;
  }

  int j = 0;
  while (j < foo(0)) {
    accumulator++;
  }

  do {
    accumulator++;
  } while (j < foo(0));

  // ==== Assignments ====
  int NotThreadID = foo(0);

  while (j < NotThreadID) {
    accumulator++;
  }

  do {
    accumulator++;
  } while (j < NotThreadID);

  int NotThreadIDAssigned = 0;
  NotThreadIDAssigned = foo(0) * 2;

  for (int i = 0; i < NotThreadIDAssigned; i++) {
    accumulator++;
  }

  struct { int NotIDDepField; } Example;
  Example.NotIDDepField = foo(0);

  for (int i = 0; i < Example.NotIDDepField; i++) {
    accumulator++;
  }

  while (j < Example.NotIDDepField) {
    accumulator++;
  }

  do {
    accumulator++;
  } while (j < Example.NotIDDepField);

  // ==== Inferred Assignments ====
  int NotThreadIDVarFromVar = NotThreadID * 2;
  for (int i = 0; i < NotThreadIDVarFromVar; i++) {
    accumulator++;
  }

  int NotThreadIDVarAssignFromVar;
  NotThreadIDVarAssignFromVar = NotThreadID * 2;
  for (int i = 0; i < NotThreadIDVarAssignFromVar; i++) {
    accumulator++;
  }

  int NotThreadIDVarFromMember = Example.NotIDDepField * 2;
  for (int i = 0; i < NotThreadIDVarFromMember; i++) {
    accumulator++;
  }

  int NotThreadIDVarAssignFromMember;
  NotThreadIDVarAssignFromMember = Example.NotIDDepField * 2;
  for (int i = 0; i < NotThreadIDVarAssignFromMember; i++) {
    accumulator++;
  }

  struct { int NotIDDepFieldFromVar; } ExampleFromVar;
  ExampleFromVar.NotIDDepFieldFromVar = NotThreadID * 2;
  for (int i = 0; i < ExampleFromVar.NotIDDepFieldFromVar; i++) {
    accumulator++;
  }

  struct { int NotIDDepFieldFromMember; } ExampleFromMember;
  ExampleFromMember.NotIDDepFieldFromMember = Example.NotIDDepField * 2;
  for (int i = 0; i < ExampleFromMember.NotIDDepFieldFromMember; i++) {
    accumulator++;
  }

  // ==== Unused Inferred Assignments ====
  int UnusedNotThreadID = Example.NotIDDepField;

  struct { int NotIDDepField; } UnusedStruct;
  UnusedStruct.NotIDDepField = NotThreadID * 2;
}
