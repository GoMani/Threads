
#include<iostream>
using namespace std;


class Node
{
	public:
		int i;
		Node* next;
		Node()
		{
			next=NULL;
			i=0;
		}
};
typedef  class Node *  NodePtr;

class List
{


	NodePtr Head;
	public :
	NodePtr getHead(){return Head;}
	void  setHead(NodePtr t){Head=t;}
	NodePtr simpleReverse(NodePtr temp)
	{
		if(temp->next!=NULL)
		{
			cout<<"pushing "<<temp<<"   "<<temp->i<<endl;

			NodePtr tmp=simpleReverse(temp->next);
			tmp->next=temp;
			temp->next=NULL;
			return temp;
		}
		else
		{
			Head=temp;
			return temp;
		}
	}

	NodePtr reverse1(NodePtr temp)
	{
		if(temp->next!=NULL)
		{
			cout<<"pushing "<<temp<<"   "<<temp->i<<endl;

			NodePtr tmp=reverse1(temp->next);
			NodePtr head=tmp->next;  
			tmp->next=temp;
			temp->next=head;
			return temp;
		}
		else
		{
			temp->next=temp;
			return temp;
		}
	}

/*************************************************************************************************
 * Description Reverse a Linked List and return the Head.
 *   Ex:
 *       Node* p=reverse(list).
 *       p shuld have the head of the reversed linked list
 *
 * Input List Head
 * Ouput Reveresed List Head
 *  
 *  cnt used for Depth of the stack 
 *  Used to find all pushn and pop are done
 *
 *  tail is carried in the return  nodepointer->next 
 *
 *
 * At End,  Head is returned instead of Tail
 *
 ***************************************************************************************************/
	NodePtr reverse(NodePtr temp)
	{
		static int cnt=0;
		if(temp->next!=NULL)
		{	cnt++;
			NodePtr tmp=reverse(temp->next);
			cnt--;
			NodePtr head=tmp->next;  
			tmp->next=temp;
			if(cnt==0)
			{
				temp->next=NULL;
				return head;
			}
			else {
				temp->next=head;
			}
			return temp;
		}
		else
		{
			temp->next=temp;
			return temp;
		}
	}

	void insert(int i)
	{
		NodePtr curr=Head;
		NodePtr temp=new Node();
		temp->i=i;
		if(Head==NULL)
			Head=temp;
		else
		{
			while (curr!=NULL && curr->next) curr=curr->next;
			curr->next=temp;
		}

	}

	void print()
	{
		NodePtr current =Head;
		while(current)
		{
			cout<<current->i<<endl;
			current=current->next;
		}
	}
	void print(NodePtr temp)
	{
		NodePtr current ;Head=temp;
		current=Head;
		while(current)
		{
			cout<<current->i<<endl;
			current=current->next;
		}
	}
	~List()
	{
		NodePtr temp;
		while(Head) {
			temp=Head;
			Head=Head->next;
			delete temp;
			temp=NULL;
		}

	}

};


int main()
{
	List myList;
	myList.insert(1);
	myList.insert(2);
	myList.insert(3);
	myList.insert(4);
	myList.insert(5);
	myList.print();
	/*cout<<"Head"<<myList.getHead()<<endl;
	  NodePtr temp=myList.reverse1(myList.getHead());
	  cout<<"Head"<<myList.getHead()<<endl;
	  cout<<"temp"<<temp->next<<endl;
	  myList.setHead(temp->next);
	  temp->next=NULL;
	  myList.print();*/
	NodePtr temp=myList.reverse(myList.getHead());
	myList.setHead(temp);
	myList.print();
	return 0;
}

