#include <QtCore/QCoreApplication>
#include <QString>
#include <QStringList>
#include <QRect>
#include <QPoint>
#include <QList>
#include <QMap>
#include <QDebug>

class State {
public:
	State() {}
	State(const QStringList &s);
	State(const QVector<QVector<int> > &s);
	State(const State &s): s(s.s) {}

	QList<State> expand() const;

	//сравнение двух состояний
	bool operator == (const State &s) const;

	// печать состояния
	operator QString() const { return s; }
	QString toString() const { return s; }
private:
	QPoint horse_index();

	// 0 - пустая / 1 - пешка / 7 - конь
	QVector<QVector<int> > field;
};

State::State(const QStringList &s): field(8) {
	QStringList::iterator i;
	for (int k = 0, i = s.begin(); i != s.end(); ++i, ++k) {
		QString t = (*i);
		for (int j = 0; j < t.length(); ++j) {
			switch(t[j]) {
			case '.':
				field[k].push_back(0);
				break;
			case 'P':
				field[k].push_back(1);
				break;
			case 'K':
				field[k].push_back(7);
				break;
			}
		}
	}
}

State::State(const QVector<QVector<int> > &s): field(s) {
}

bool State::operator == (const State &s) const {
	return field == s.field;
}

QPoint State::horse_index() {
	for (int i = 0; i < field.size(); ++i) {
		int index = field[i].indexOf(7);
		if (index != -1)
			return QPoint(i, index);
	}
	return QPoint(-1, -1);
}

QList<State> State::expand() const {
	QList<State> l;

	QRect f(0,0,7,7);

	QPoint horse = horse_index();

	QPoint n = horse + QPoint(1,3);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}
	n = horse + QPoint(-1,3);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}

	n = horse + QPoint(-3,1);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}

	n = horse + QPoint(-3,-1);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}

	n = horse + QPoint(1,-3);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}
	n = horse + QPoint(-1,-3);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}

	n = horse + QPoint(3,1);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}
	n = horse + QPoint(3,-1);
	if (f.contains(n)) {
		QVector<QVector<int> > t = field;
		t[horse.y()][horse.x()] = 0;
		t[n.y()][n.x()] = 7;
		l.append(State(t));
	}

	return l;
}


class DeepThought {
public:
	DeepThought(const State &begin, const State &end);

	// Answer to the Ultimate Question of Life, the Universe, and Everything
	int answer();

	void backtrace();
private:
	State begin;
	State end;

	QMap<State, State> tree;
};

DeepThought::DeepThought(const State &begin, const State &end)
	: begin(begin), end(end) {
	tree.insert(begin, end);
}

int DeepThought::answer() {
	QVector<State> iters;
	iters.append(tree.begin().key());

	for (int i = 0; i < iters.size(); ++i) {
		QList<State> l = iters[i].expand();
		if (l.isEmpty())
			continue;
		QList<State>::const_iterator j;

		for (j = l.begin(); j != l.end(); ++j) {
			if (tree.find((*j)) == tree.end()) {
				iters.append((*j));
				tree.insert( (*j), iters[i] );
				if ((*j) == end)
					return 42;
			}
		}
	}
	return 0;
}

void DeepThought::backtrace() {
	pair<State, State> s = tree.back();
	while (! (s.first == begin)) {
		o << s.first << endl;
		list< pair<State, State> >::iterator i = find(s.second);
		if (i == tree.end())
			break;
		s = (*i);
	}
}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	DeepThought task(State("oooo.xxxx"), State("xxxx.oooo"));
	task.answer();
	//task.backtrace(cout);

	return a.exec();
}

