void Sem_Up(struct sembuf oper,int semNum, int semid);
void Sem_Down(struct sembuf oper,int semNum, int semid);

int switchTurn(int turn);