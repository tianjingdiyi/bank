#include<stdio.h>
#include<stdlib.h>
#include <string>
#include <time.h>
using namespace std;

int total; //��ʼʱ�����ִ��ʽ��ܶ�
int closeTime; //Ӫҵ����ʱ��
int arriveTime; //���������¼�֮��ļ������
int dealTime; //�ͻ�֮�佻�׵�ʱ������
int dealMoney = 5000; //���׶�����
int currentTime = 0; //��ǰʱ��
int totalTime = 0; //�ͻ�������ʱ��
int counter = 0; //�ͻ�����
int number = 1; //��ʼ�ͻ����к�

struct service {
	int num; //�ͻ���
	string type; //������뿪
	int beginTime;
	int endTime;
	int money; //����Ϊ������Ϊȡ��
	service* next;
};

struct queue { //����
	service* head;
	service* rear;
};

void push(queue &q,int d) {// ����Ԫ��dΪQ���µĶ�βԪ��
	service* temp = new service;
	temp->money = d;
	temp->next = NULL;
	if(NULL == q.head) {//����Ϊ�գ���ʼ��
		q. head = temp;
		q. rear = temp;
	}//if
	else {//���в�Ϊ�գ�����Ԫ��d
		q. rear->next = temp;
		q. rear = q.rear->next;
	}//else
}

void pop(queue &q) {// �����в���,�����к���
	service* temp;
	temp = q. head;
	if(NULL ==q. head->next)
		q.head = q. rear =NULL;
	else
		q. head=q. head->next;
	delete temp;
}
service* front(queue &q) {//���ض���Ԫ��
	return q. head;
}

service* back(queue &q) {//���ض�βԪ��
	return q. rear;
}

service* searchAndDel(queue &q,int m) {//�ڶ�����Ѱ�ҿɴ���Ԫ��
	service* sign = q. head; //���ͷ�ڵ�
	service* temp;
	while(NULL != q. head) {
		if((-(q. head->money)) <m) {//����Ԫ�ؿ��Դ���
			if(q. head==q.rear) {
				temp = q. head;
				q. head = q. rear = NULL;
				return temp;
			}
			else {//����Ԫ�س���
				temp = q. head; 
				q. head = q. head->next; // �׽ڵ����һλ,����ԭ�׽ڵ�
				return temp;
			}
		}
		else {//����Ԫ�ײ��ܱ�����
			if(q. head == q. rear){}
			else {//�׽ڵ��Ƶ�����β��
				q. rear->next = q. head; 
				q. rear = q. rear->next;
				q. head =q. head->next;
				q. rear->next = NULL;
			}
		}
		if(q. head == sign)//����ѭ��һ��ʱֹͣ
			return NULL;	
	}	
	return NULL;
}
bool state =1; //�����ж��Ƿ��д����ڴ���
int currentTimeOfDeal = 0;
int theArriveTime = 0;
queue eq; //�¼�����
queue fq; //����һ
queue sq; //���ж�
//��ʼ����������
void arrive() {/*"����"���� ��������˿ͣ��������һ
���������¼� �����¼�����*/
	push(fq,(rand()% (2*dealMoney) -dealMoney)); //��������˿ͼ����һ����
	back(fq)->beginTime = currentTime;
	back(fq)->num = number;
	push(eq,(back(fq)->money)); //�������¼������¼�����
	back(eq)->beginTime = currentTime;
	back(eq)->type = "����";
	back(eq)->num = number;	
	++number;
}

void putMoney() { //����
	total += front(fq)->money; //�����ʽ��ܶ�
	push(eq,front(fq)->money); //�����¼����� �뿪
	back(eq)->type = "�뿪";
	back(eq)->num = front(fq)->num;
	back(eq)->endTime = (front(fq)->beginTime + rand()%dealTime +1);
	++counter; //���¿ͻ�����
	totalTime += (back(eq)->endTime - front(fq)->beginTime); //���¶���ʱ��
	pop(fq); //ɾ����һ���е�һ��ҵ��
	currentTimeOfDeal = back(eq)->endTime; 
	state =0;
}

void getMoney() {//ȡ���
	if( (-fq.head->money) > total ) {//�ʽ��ȱ ����ڶ�����
		push( sq,front(fq)->money );
		back(sq)->beginTime = front(fq)->beginTime;
		back(sq)->num = front(fq)->num;
		pop(fq);
	}
	else {
		total += back(fq)->money;
		push(eq,front(fq)->money); //�����¼����� �뿪
		back(eq)->type = "�뿪";
		back(eq)->num = front(fq)->num;
		back(eq)->endTime = (front(fq)->beginTime + rand()%dealTime +1);
		back(eq)->beginTime = 0;
		currentTimeOfDeal = back(eq)->endTime;
		++counter; //���¿ͻ����� 
		totalTime += ( back(eq)->endTime - back(fq)->beginTime ); //���¶���ʱ��
		pop(fq); //ɾ����һ���е�һ��ҵ��
		state =0;
	}
}
service* temped ;
int randomTemp;
void findAndDeal() {//�ڶ�����Ѱ�ҿɴ���Ԫ�أ�������д���
	while( (temped= searchAndDel(sq,total))&&NULL != temped ) {//���ҿɴ���ȡ��
		total += temped->money; //�����ʽ��ܶ�
		push(eq,temped->money); //�����¼�����ѵ �뿪
		back(eq)->type = "�뿪";
		back(eq)->num = temped->num;
		randomTemp = rand()%dealTime +1;
		back(eq)->endTime = currentTime + randomTemp ;
		currentTimeOfDeal += randomTemp;
		++counter; //���¿ͻ�����
		totalTime += ( back(eq)->endTime - temped->beginTime ); //���¶���ʱ�� 
		delete temped; //ɾ���ڵ�
		temped = NULL;	
	}	
	state = 0;
}
int main() { 
    printf("\n\n");
	printf("                ********************************************\n");
	printf("                *                                          *\n");
	printf("                *           ��ӭ��������ģ��ϵͳ           *\n");
	printf("                *                                          *\n");
	printf("                *             1.��ʼģ�� 0.�˳�            *\n");
	printf("                *                                          *\n");
	printf("                ********************************************\n");
	int n;
	printf("���������ѡ��");
	scanf("%d",&n);
	while(n==1) {
		srand(time(NULL)); //��ʼ���������
		printf("�������еĳ�ʼ��\n");
		scanf("%d",&total);
		printf("�������е�Ӫҵʱ�䣺\n");
		scanf("%d",&closeTime);
		printf("������󵽴�ʱ������\n"); 
		scanf("%d",&arriveTime);
		printf("�������Ĵ���ʱ�䣺\n");
		scanf("%d",&dealTime);
		theArriveTime +=rand()%arriveTime + 1; //�״ε���ʱ��
		while(currentTime < closeTime) {
			++currentTime;
			if( currentTimeOfDeal < currentTime ) currentTimeOfDeal = currentTime ;
			if( currentTimeOfDeal == currentTime ) state = 1;
			if( currentTime == theArriveTime ) {//�����¼�
				arrive();
				theArriveTime +=rand()%arriveTime +1; 
			}
			if( 1 == state && NULL != fq.head) {
				if(fq.head->money >= 0) {
					putMoney();
					findAndDeal();
				}
				else 
				getMoney();
			}
		}
		printf("�ͻ�����ƽ��ʱ��Ϊ: %d\n",totalTime/counter);
		printf("���е�ǰ���: %d\n",total);
		break;
	}
	return 0;
}

