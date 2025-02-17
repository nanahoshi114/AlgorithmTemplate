class list{
    class Node{
    public:
        Node *next;
        int data;
        Node(int val) : next(nullptr), data(val){};
    };
    Node *head;
public:
    list(){
        head = nullptr;
    }
    void push_front(int val){
        if(!head){
            head = new Node(val);
        }else{
            Node *now = new Node(val);
            now->next = head;
            head = now;
        }
    }
    void pop_front(){
        if(!head) return;
        Node *tmp = head->next;
        delete head;
        head = tmp;
    }
    void push_back(int val){
        if(!head){
            head = new Node(val);
        }else{
            Node *now = head;
            while(now->next) now = now->next;
            now->next = new Node(val);
        }
    }
    void insert(Node *pos, int val){
        Node *now = new Node(val);
        if(!pos->next){
            pos->next = now;
        }else{
            now->next = pos->next;
            pos->next = now;
        }
    }
    int query(int ind){
        Node *now = head;
        while(ind-- && now->next) now = now->next;
        return now->data;
    }
    void erase(Node *pos){
        if(!pos->next) return ;
        Node *tmp = pos->next;
        pos->next = tmp->next;
        delete tmp;
    }
    void clear(){
        Node *tmp = head;
        while(head){
            tmp = head->next;
            delete head;
            head = tmp;
        }
    }
};