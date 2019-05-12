#include <stdio.h>
#include <stdlib.h>

typedef struct DFA DFA;
typedef struct set_list set_list;
typedef struct set set;

struct DFA{
	size_t inp_count;
	size_t state_count;
	size_t f_state_count;
	int *inputs;
	char *states;
	char initial_state;
	char *final_states;
	char **element;
	int *isvisited;
};

struct set_list{
	size_t set_count;
	set *sets;
};

struct set{
	size_t state_count;
	char *states;
};

DFA create_dfa(size_t,size_t,char,const char*);
void set_input(DFA*,int,unsigned int);
void set_initial(DFA*,char);
void set_data(DFA*,char,unsigned int,char);
int is_init(DFA,char);
int is_final(DFA,char);
int is_dead(DFA,char);
void minimize(DFA*);
void print_dfa(DFA);
set_list get_list(DFA);

set_list create_list();
void add_set(set_list*,set);
int equivalance(set_list,char,char);
set_list level_up(set_list,DFA);
void print_list(set_list);

set create_set(const char*);
void add_state(set*,char);
void remove_state(set*,char);
int is_have(set,char);
void print_set(set);

int main(){
	DFA t = create_dfa(3,3,'A',"C");
	set_data(&t,'A',0,'A');
	set_data(&t,'A',1,'B');
	set_data(&t,'A',2,'A');
	set_data(&t,'B',0,'B');
	set_data(&t,'B',1,'C');
	set_data(&t,'B',2,'A');
	set_data(&t,'C',0,'C');
	set_data(&t,'C',1,'A');
	set_data(&t,'C',2,'B');
	printf("%d",is_dead(t,'C'));
/*	print_dfa(t);
	set_list list = get_list(t);
	for(int i = 0;i < 10;i++){
		print_list(list);
		printf("\n\n");
		list = level_up(list,t);
	}*/
}

DFA create_dfa(size_t state_c,size_t inp_c,char init,const char *finals){
	DFA t;
	t.inp_count = inp_c;
	t.state_count = state_c;
	t.f_state_count = 0;
	t.inputs = malloc(sizeof(int) * inp_c);
	for(int i = 0;i < inp_c;i++)t.inputs[i] = i;
	t.states = malloc(sizeof(char) * state_c);
	t.initial_state = init;
	while(*finals){
		t.f_state_count++;
		t.final_states = realloc(t.final_states,sizeof(char) * t.f_state_count);
		t.final_states[t.f_state_count - 1] = *(finals++);
	}
	t.element = malloc(sizeof(char*) * state_c);
	for(int i = 0;i < state_c;i++){
		t.states[i] = i + 65;
		t.element[i] = calloc(inp_c,sizeof(char));
	}
	t.isvisited = calloc(state_c,sizeof(int));
	return t;
}

void set_input(DFA *t,int inp,unsigned int order){
	t->inputs[order] = inp;
}

void set_initial(DFA *t,char state){
	t->initial_state = state;
}

void set_data(DFA *t,char state,unsigned int input,char value){
	t->element[state - 65][input] = value;
}

int is_init(DFA t,char state){
	return (t.initial_state == state)? 1 : 0;
}

int is_final(DFA t,char state){
	for(int i = 0;i < t.f_state_count;i++){
		if(t.final_states[i] == state){
			return 1;
		}
	}
	return 0;
}

int is_dead(DFA t,char state){
	if(!is_init(t,state)){
		t.isvisited[state - 65] = 1;
		for(int i = 0;i < t.state_count;i++){
			for(int j = 0;j < t.inp_count;j++){
				if(t.element[i][j] == state && !t.isvisited[i]){
					if(is_dead(t,i + 65)){
						t.isvisited = calloc(t.state_count,sizeof(int));
						return 0;
					}
				}
			}
		}
		t.isvisited = calloc(t.state_count,sizeof(int));
		return 1;
	}
	t.isvisited = calloc(t.state_count,sizeof(int));
	return 0;
}

void minimize(DFA *t){
		
}

void print_dfa(DFA t){
	printf("Q : {");
	for(int i = 0;i < t.state_count;i++)printf("%c, ",t.states[i]);
	printf("}\n");
	
	printf("E : {");
	for(int i = 0;i < t.inp_count;i++)printf("%d, ",t.inputs[i]);
	printf("}\n");

	printf("q0: %c\n",t.initial_state);

	printf("F : {");
	for(int i = 0;i < t.f_state_count;i++)printf("%c, ",t.final_states[i]);
	printf("}\n\n");

	printf("\t");
	for(int i = 0;i < t.inp_count;i++){	
		printf("%d\t",t.inputs[i]);
	}
	printf("\n");
	for(int i = 0;i < t.state_count;i++){
		if(is_init(t,t.states[i])){
			printf("->%c\t",t.states[i]);
		}else if(is_final(t,t.states[i])){
			printf(" *%c\t",t.states[i]);
		}else{
			printf("  %c\t",t.states[i]);
	
		}
		for(int j = 0;j < t.inp_count;j++){
			printf("%c\t",t.element[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

set_list get_list(DFA t){
	set_list list = create_list();
	add_set(&list,create_set(0));
	add_set(&list,create_set(0));
	for(int i = 0;i < t.state_count;i++){
		if(is_final(t,t.states[i])){
			add_state(&list.sets[1],t.states[i]);
		}else{
			add_state(&list.sets[0],t.states[i]);
		}
	}
	return list;
}

set_list create_list(){
	set_list t;
	t.set_count = 0;
	t.sets = 0;
	return t;
}

void add_set(set_list *list,set s){
	list->set_count++;
	list->sets = realloc(list->sets,sizeof(set) * list->set_count);
	list->sets[list->set_count - 1] = s;
}

int equivalance(set_list list,char s1,char s2){
	for(int i = 0;i < list.set_count;i++){
		if(is_have(list.sets[i],s1) && is_have(list.sets[i],s2)){
			return 1;
		}
	}
	return 0;
}

set_list level_up(set_list list,DFA t){
	set_list new_list = create_list();
	for(int i = 0;i < list.set_count;i++){
		set s = list.sets[i];
		if(s.state_count > 1){
			int *used = calloc(s.state_count,sizeof(int));
			for(int j = 0;j < s.state_count;j++){;
				if(!used[j]){
					set tmp = create_set(0);
					add_state(&tmp,s.states[j]);
					for(int k = j + 1;k < s.state_count;k++){
						int equality = 1;
						for(int l = 0;l < t.inp_count;l++){
							if(!equivalance(list,t.element[s.states[j] - 65][l],t.element[s.states[k] - 65][l])){
								equality = 0;
								break;
							}
						}
						if(equality){
							add_state(&tmp,s.states[k]);
							used[k] = 1;
						}
					}
					add_set(&new_list,tmp);
				}	
			}
		}else{
			add_set(&new_list,s);
		}
	}
	return new_list;
}

void print_list(set_list list){
	for(int i = 0;i < list.set_count;i++){
		printf("{");
		print_set(list.sets[i]);
		printf("}, ");
	}
}

set create_set(const char *states){
	set s;
	s.state_count = 0;
	s.states = 0;
	if(states){
		while(*states){
			s.state_count++;
			s.states = realloc(s.states,sizeof(set) * s.state_count);
			s.states[s.state_count - 1] = *states++;
		}
	}
	return s;
}

void add_state(set *s,char state){
	if(!is_have(*s,state)){
		s->state_count++;
		s->states = realloc(s->states,sizeof(char) * s->state_count);
		s->states[s->state_count - 1] = state;
	}
}

void remove_state(set *s,char state){
	int shift = 0;
	for(int i = 0;i < s->state_count - shift;i++){
		while(s->state_count > i + shift && s->states[i + shift] == state){
			shift++;	
		}
		if(i + shift < s->state_count){
			s->states[i] = s->states[i + shift];
		}
	}
	s->state_count -= shift;
	s->states = realloc(s->states,sizeof(char) * s->state_count);
}

int is_have(set set,char s){
	for(int i = 0;i < set.state_count;i++){
		if(set.states[i] == s)return 1;
	}
	return 0;
}

void print_set(set s){
	for(int i = 0;i < s.state_count;i++)printf("%c, ",s.states[i]);
}
