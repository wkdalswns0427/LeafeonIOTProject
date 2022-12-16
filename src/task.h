#ifndef task_H
#define task_H

void TaskCCS811(void *pvParameters);
void TaskPMS7003(void *pvParameters);
void TaskBME280(void *pvParameters);
void ServerTask(void *pvParameters);

#endif
