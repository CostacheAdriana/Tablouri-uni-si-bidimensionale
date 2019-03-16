#include <iostream>

using namespace std;

class Vector{

    private:
        int dim;
        int* a;

    public:
        Vector();
        Vector(int);
        Vector(int, int*);
        Vector(const Vector&);
        ~Vector();

        friend ostream& operator<<(ostream& out, const Vector& v){

            for(int i = 0;i < v.dim; ++i){
                out << v[i] << " ";
            }

            return out;
        }

        friend istream& operator>>(istream& in, Vector& v){

            for(int i = 0;i < v.dim; ++i){
                cout << "Elementul " << i << " : ";
                in >> v.a[i];
            }

            return in;
        }

        Vector operator=(const Vector&);
        int& operator[](int);
        int operator[](int) const;
};

class Matrice{

    protected:
        Vector** v;
        int nr_linii;

    public:
        Matrice();
		Matrice(int);
        Matrice(Vector**, int);
        Matrice(const Matrice&);

		//destructor virtual pentru a apela toti destructorii
        virtual ~Matrice();

        virtual void scrie(ostream&) = 0;
        virtual void scrie(ostream&) const = 0;
        virtual void citeste(istream&) = 0;

        friend ostream& operator<<(ostream& out, const Matrice* m){
            m->scrie(out);
            return out;
        }

        friend istream& operator>>(istream& in, Matrice* m){
            m->citeste(in);
            return in;
        }

        void operator=(const Matrice&);
        Vector& operator[](int);
        Vector operator[](int) const;

		virtual bool eDiagonala(){ return false; }
};

class Matrice_oarecare : public Matrice{

    private:
        int nr_coloane;

    public:
        Matrice_oarecare();
        Matrice_oarecare(Vector**, int, int);
        Matrice_oarecare(const Matrice_oarecare&);
        ~Matrice_oarecare();

        virtual void scrie(ostream&);
        virtual void scrie(ostream&) const;
        virtual void citeste(istream&);

        Matrice_oarecare operator=(const Matrice_oarecare&);

		virtual bool eDiagonala();
};

class Matrice_patratica : public Matrice{

    public:
        Matrice_patratica();
		Matrice_patratica(int);
        Matrice_patratica(Vector**, int);
        Matrice_patratica(const Matrice_patratica&);
        ~Matrice_patratica();

		virtual void scrie(ostream&){}
        virtual void scrie(ostream&) const;
        virtual void citeste(istream&);      

        int calculeazaDeterminant();
		int calculeazaDeterminant() const;

        Matrice_patratica operator=(const Matrice_patratica&);

        bool eSuperiorTriunghiulara();
        bool eInferiorTriunghiulara();
        bool eDiagonala();

	private:
		int semn(int, int);
		int semn(int, int) const;

		int max(int, int);
		int max(int, int) const;
};

Vector::Vector(){}

//constructor pentru vector 0
Vector::Vector(int dim){

    this->dim = dim;
    a = new int[this->dim];

    for(int i = 0;i < this->dim; ++i){
        a[i] = 0;
    }
}

//constructor pt initializare
Vector::Vector(int dim, int* a){

    a = new int[dim];

    for(int i = 0;i < dim; ++i){
        this->a[i] = a[i];
    }
}

//copy constructor
Vector::Vector(const Vector& v){

    this->dim = v.dim;
    this->a = new int[this->dim];

    for(int i = 0;i < this->dim; ++i){
        this->a[i] = v[i];
    }
}

Vector::~Vector(){
    delete[] a;
}

Vector Vector::operator=(const Vector& v){

	//verifica daca e cazul a = a
    if(this == &v){
        return (*this);
    }

    this->dim = v.dim;

	//creeaza un nou vector
    delete[] this->a;
    this->a = new int[this->dim];

	//copiaza elementele
    for(int i = 0;i < this->dim; ++i){
        this->a[i] = v[i];
    }
}

//returneaza elementul de pe pozitia index din vector.Verifica daca indexul e in limite
int Vector::operator[](int index) const{
    
    try{
        if(index >= this->dim){
            throw index;
        }
        return this->a[index];
    }catch(int e){
        cout << index << " depaseste dimensiunile\n";
    }
}

int& Vector::operator[](int index){

    try{
        if(index >= this->dim){
            throw index;
        }
        return this->a[index];
    }catch(int e){
        cout << index << " depaseste dimensiunea vectorului\n";
    }
}

Matrice::Matrice(){
    v = NULL;
}

//constructor pentru matrice neinitializata
Matrice::Matrice(int nr_linii){
	this->nr_linii = nr_linii;
	this->v = new Vector*[nr_linii];

	for (int i = 0; i < this->nr_linii; ++i){
		this->v[i] = new Vector(nr_linii);
	}
	
}

//constructor pt matrice initializata
Matrice::Matrice(Vector** v, int nr_linii){

    this->nr_linii = nr_linii;
    this->v = new Vector*[nr_linii];

    for(int i = 0;i < this->nr_linii; ++i){
        this->v[i] = new Vector(*v[i]);
    }
}

//copy constructor
Matrice::Matrice(const Matrice& m){

    this->nr_linii = m.nr_linii;
    this->v = new Vector*[m.nr_linii];

    for(int i = 0;i < this->nr_linii; ++i){
        this->v[i] = new Vector(*v[i]);
    }
}

//sterge fiecare vector,apoi array-ul de pointeri catre obiecte de tip Vector
Matrice::~Matrice(){
    
    for(int i = 0;i < this->nr_linii; ++i){
        delete this->v[i];
    }

    delete[] this->v;
}

void Matrice::operator=(const Matrice& m){

    this->nr_linii = m.nr_linii;

    for(int i = 0;i < this->nr_linii; ++i){
        delete this->v[i];
    }

    delete[] v;

    this->v = new Vector*[this->nr_linii];
    for(int i = 0;i < this->nr_linii; ++i){
        this->v[i] = new Vector();
        this->v[i] = m.v[i];
    }
}

//returneaza linia #index
Vector& Matrice::operator[](int index){
    return (*this->v[index]);
}

Vector Matrice::operator[](int index) const{
    return (*this->v[index]);
}

Matrice_oarecare::Matrice_oarecare() : Matrice(NULL, 0){}

//contructor pentru initializarea matricei
Matrice_oarecare::Matrice_oarecare(Vector** v, int nr_linii, int nr_coloane) : Matrice(v, nr_linii){
    this->nr_coloane = nr_coloane;
}

Matrice_oarecare::~Matrice_oarecare(){}

//copy constructor
Matrice_oarecare::Matrice_oarecare(const Matrice_oarecare& m) : Matrice(m){
    this->nr_coloane = m.nr_coloane;
}

Matrice_oarecare Matrice_oarecare::operator=(const Matrice_oarecare& m){
    
    if(this == &m){
        return (*this);
    }

	//apeleaza operatorul din clasa de baza
    Matrice::operator=(m);
    return (*this);
}

//nu se aplica la matrici oarecare
bool Matrice_oarecare::eDiagonala(){
	cout << "nu se aplica pt matrici oarecare";
	return false;
}


//functia de afisare 
void Matrice_oarecare::scrie(ostream& out){

    for(int i = 0;i < nr_linii; ++i){
        for(int j = 0;j < nr_coloane; ++j){
            out << (*this)[i][j] << " ";
        }
        out << "\n";
    }
}

void Matrice_oarecare::scrie(ostream& out) const{

    for(int i = 0;i < nr_linii; ++i){
        for(int j = 0;j < nr_coloane; ++j){
            out << (*this)[i][j] << " ";
        }
        out << "\n";
    }
}

//functie de citire specifica Matrice_oarecare
void Matrice_oarecare::citeste(istream& in){

    cout << "numar linii : ";
    in >> this->nr_linii;

    cout << "numar coloane : ";
    in >> this->nr_coloane;

    this->v = new Vector*[nr_linii];
    for(int i = 0;i < this->nr_linii; ++i){
        this->v[i] = new Vector(this->nr_coloane);
        cout << "linia " << i << " :\n";
        in >> *(this->v[i]);
    }
}

Matrice_patratica::Matrice_patratica() : Matrice(NULL, 0){}
Matrice_patratica::Matrice_patratica(int nr_linii) : Matrice(nr_linii){}

//constructor care initializeaza matricea
Matrice_patratica::Matrice_patratica(Vector** v, int nr_linii) : Matrice(v, nr_linii){}

Matrice_patratica::~Matrice_patratica(){}

Matrice_patratica::Matrice_patratica(const Matrice_patratica& m) : Matrice(m){}

Matrice_patratica Matrice_patratica::operator=(const Matrice_patratica& m){

    if(this == &m){
        return (*this);
    }

	//apeleaza operatorul din clasa de baza
    Matrice::operator=(m);
    return (*this);
}

//verific daca deasupra diagonalei am elemente dif de 0
bool Matrice_patratica::eInferiorTriunghiulara(){

	for (int i = 0; i < nr_linii - 1; ++i){
		for (int j = i + 1; j < nr_linii; ++j){
			if ((*this)[i][j] != 0){
				return false;
			}
		}
	}

	return true;
}

//verific daca sub diagonala am elem dif de 0
bool Matrice_patratica::eSuperiorTriunghiulara(){

	for (int i = 0; i < nr_linii; ++i){
		for (int j = 0; j < i; ++j){
			if ((*this)[i][j] != 0){
				return false;
			}
		}
	}

	return true;
}

//diagonala nenula
bool Matrice_patratica::eDiagonala(){
    return (eInferiorTriunghiulara() && eSuperiorTriunghiulara());
}

int Matrice_patratica::semn(int i, int j){
	return (((i + j) % 2) ? -1 : 1);
}

int Matrice_patratica::semn(int i, int j) const{
	return (((i + j) % 2) ? -1 : 1);
}

int Matrice_patratica::max(int i, int j){
	return (i > j) ? i : j;
}

int Matrice_patratica::max(int i, int j) const{
	return (i > j) ? i : j;
}

//calculez determinant recursiv pana la gradul 3 unde pot calcula direct
int Matrice_patratica::calculeazaDeterminant(){

	if (nr_linii == 3){
		return ((*this)[0][0] * (*this)[1][1] * (*this)[2][2] + (*this)[0][2] * (*this)[1][0] * (*this)[2][1] + (*this)[0][1] * (*this)[1][2] * (*this)[2][0] -
			    (*this)[0][2] * (*this)[1][1] * (*this)[2][0] - (*this)[0][1] * (*this)[1][0] * (*this)[2][2] - (*this)[0][0] * (*this)[1][2] * (*this)[2][1]);
	}

	int determinant = 0;

	for (int k = 0; k < nr_linii; ++k){
		Matrice_patratica aux(nr_linii - 1);
		for (int i = 1; i < nr_linii; ++i){
			for (int j = 0; j < nr_linii; ++j){
				if (j == k){
					continue;
				}
				aux[i - 1][max(0, j - (k + 1))] = (*this)[i][j];
			}
		}
		determinant += (*this)[0][k] * semn(0, k) * aux.calculeazaDeterminant();
	}
}

int Matrice_patratica::calculeazaDeterminant() const{

	if (nr_linii == 1){
		return (*this)[0][0];
	}

	if (nr_linii == 2){
		return ((*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0]);
	}

	if (nr_linii == 3){
		return ((*this)[0][0] * (*this)[1][1] * (*this)[2][2] + (*this)[0][2] * (*this)[1][0] * (*this)[2][1] + (*this)[0][1] * (*this)[1][2] * (*this)[2][0] -
			(*this)[0][2] * (*this)[1][1] * (*this)[2][0] - (*this)[0][1] * (*this)[1][0] * (*this)[2][2] - (*this)[0][0] * (*this)[1][2] * (*this)[2][1]);
	}

	int determinant = 0;

	for (int k = 0; k < nr_linii; ++k){
		Matrice_patratica aux(nr_linii - 1);
		for (int i = 1; i < nr_linii; ++i){
			for (int j = 0; j < nr_linii; ++j){
				if (j == k){
					continue;
				}
				int ind = j;
				if (j > k){
					--ind;
				}

				aux[i - 1][ind] = (*this)[i][j];
			}
		}

		determinant += (*this)[0][k] * semn(0, k) * aux.calculeazaDeterminant();
	}

	return determinant;
}

//citire pentru matrice_patratica
void Matrice_patratica::citeste(istream& in){

	cout << "dimensiune : ";
    in >> this->nr_linii;

    this->v = new Vector*[this->nr_linii];
    for(int i = 0;i < this->nr_linii; ++i){
		cout << "linia " << i << ":\n";
        this->v[i] = new Vector(this->nr_linii);
        in >> *(this->v[i]);
    }
}

//functie de scriere pentru matrice_patratica
void Matrice_patratica::scrie(ostream& out) const{

    for(int i = 0;i < nr_linii; ++i){
        for(int j = 0;j < nr_linii; ++j){
            out << (*this)[i][j] << " ";
        }
        out << "\n";
    }

    out << "Determinant : " << this->calculeazaDeterminant();
}

int main(){

    int numar_matrice = 2;
    Matrice** matrice = new Matrice*[numar_matrice];

    for(int i = 0;i < numar_matrice; ++i){
        cout << "Matrice oarecare sau patratica?";
        char answ;
        cin >> answ;
        
        if(answ == 'o'){
            matrice[i] = new Matrice_oarecare();
        }else{
            matrice[i] = new Matrice_patratica();
        }
        cin >> matrice[i];
        cout << matrice[i] << "\n";
		cout << matrice[i]->eDiagonala() << "\n";
    }

    return 0;
}
