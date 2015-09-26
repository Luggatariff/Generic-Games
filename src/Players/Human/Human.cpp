/*!
 * \file Human.cpp
 * \brief Human code
 * \author Anaël Petit
 */

#include "Human.hpp"
#include "../../Model/Coordinates.hpp"
#include <algorithm>
#include <QFormLayout>

Human::Human(){
    hu_parameter_frame=new QFrame();

    QFormLayout * form_layout = new QFormLayout(hu_parameter_frame);
    hu_parameter_frame->setLayout(form_layout);

    hu_name_line_edit=new QLineEdit("human", hu_parameter_frame);
    form_layout->addRow("Player Name :", hu_name_line_edit);

    hu_team_spin_box=new QSpinBox(hu_parameter_frame);
    form_layout->addRow("Player Team Id :", hu_team_spin_box);

    hu_team_spin_box->setMinimum(0);
    hu_team_spin_box->setMaximum(10);
}

Human::~Human(){
    delete hu_parameter_frame;
}

void Human::start(Game * game){
    cout<<"Starting new Game:"<<game->getName()<<endl;
}

void Human::end(Game * game){
	if (game->whoWon() == (int)this->getTeam()){
		cout<<"You won the Game"<<endl;
	}
}

Coordinates Human::play(Game * game, vector<Coordinates> limit_choices){
	unsigned int dimension = game->dimension();
	Coordinates result(((size_t)dimension));
	do{
		if (limit_choices.size() > 1){
			cout<<"Choose one of these Coordinates: ";
			for (vector<Coordinates>::iterator limit_choices_it = limit_choices.begin(); limit_choices_it != limit_choices.end(); limit_choices_it++){
				cout<<"("<<limit_choices_it->operator [](0);
				for (unsigned int dim = 1; dim < limit_choices_it->dimension(); dim++)
					cout<<" "<<limit_choices_it->operator [](dim);
				cout<<")";
			}
			cout<<endl;
		}
		cout<<"Choose a Square ("<<dimension<<" coordinates(s)):";
		for (unsigned int dim=0; dim<dimension; dim++)
			cin>>result[dim];
	}
	while(!game->isPlayable(result) || (limit_choices.size() > 1 && find(limit_choices.begin(), limit_choices.end(), result) == limit_choices.end()));

	return result;
}

QString Human::getName(){
    return this->hu_name_line_edit->text();
}

unsigned int Human::getTeam(){
    return (unsigned int)this->hu_team_spin_box->value();
}

QString Human::getType(){
    return "Human";
}

Player * Human::createInstance(){
    Human * result = new Human();
    return (Player *)result;
}

QFrame * Human::getParameterFrame(){
    return hu_parameter_frame;
}
