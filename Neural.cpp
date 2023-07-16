#include<iostream>
#include<list>
#include <random>
using namespace std;
class Neuron;
class Dendrite
{
private:
double weight;
Neuron *inputNeuron;
public:
Dendrite()
{
this->weight=0.0;
this->inputNeuron=NULL;
}
Dendrite(const Dendrite &other)
{
this->weight=other.weight;
this->inputNeuron=other.inputNeuron;
}
Dendrite & operator=(Dendrite other)
{
this->weight=other.weight;
this->inputNeuron=other.inputNeuron;
return *this;
}
virtual ~Dendrite()
{ // do nothing
}
void setWeight(double weight)
{
this->weight=weight;
}
double getWeight()
{
return this->weight;
}
void setInputNeuron(Neuron *inputNeuron)
{
this->inputNeuron=inputNeuron;
}
Neuron * getInputNeuron()
{
return this->inputNeuron;
}
};

class Neuron
{
private:
double output;
double error;
list<Dendrite *> dendrites;
public:
Neuron()
{
this->output=0;
this->error=0;
}
Neuron(const Neuron &other)
{
this->output=other.output;
this->error=other.error;
this->dendrites=other.dendrites;
}
virtual ~Neuron()
{
// do nothing
}
Neuron & operator=(Neuron other)
{
this->output=other.output;
this->error=other.error;
this->dendrites=other.dendrites;
return *this;
}
void setOutput(double output)
{
this->output=output;
}
double getOutput()
{
return this->output;
}
void setError(double error)
{
this->error=error;
}
double getError()
{
return this->error;
}
void addDendrite(Dendrite *dendrite)
{
this->dendrites.push_back(dendrite);
}
Dendrite * getDendrite(int index)
{
list<Dendrite *>::iterator listIterator=dendrites.begin();
advance(listIterator,index);
return *listIterator;
}
int getDendritesCount()
{
return this->dendrites.size();
}
private:
list<Dendrite *> * getDendrites()
{
return &(this->dendrites);
}
friend class Network;
};

class Network
{
private:
list<list<Neuron *> *> layers;
list<double> labels;
double (*activationFunction)(double);
public:
Network(double (*fun)(double))
{
this->activationFunction=fun;
}
Network(const Network &other)
{
}
Network & operator=(Network other)
{
return *this;
}
virtual ~Network()
{
// do nothing
}
void setNumberOfLayers(int numberOfLayers)
{
for(int x=0;x<numberOfLayers;x++) layers.push_back(new list<Neuron *>);
}
void setNumberOfNeurons(int layerNumber,int numberOfNeurons)
{
Neuron **neuronsOnPreviousLayer=NULL;
int numberOfNeuronsOnPreviousLayer;
if(layerNumber>1)
{
list<list<Neuron *> *>::iterator i=layers.begin();
advance(i,layerNumber-2);
list<Neuron *> *lastPopulatedLayer=*i;
numberOfNeuronsOnPreviousLayer=lastPopulatedLayer->size();
neuronsOnPreviousLayer=new Neuron *[numberOfNeuronsOnPreviousLayer];
list<Neuron *>::iterator ni=lastPopulatedLayer->begin();
int k=0;
while(ni!=lastPopulatedLayer->end())
{
neuronsOnPreviousLayer[k]=*ni;
k++;
++ni;
}
}
list<Neuron *> *layer;
list<list<Neuron *> *>::iterator layerIterator=layers.begin();
advance(layerIterator,layerNumber-1);
layer=*layerIterator;
Neuron *neuron;
Dendrite *dendrite;
for(int x=0;x<numberOfNeurons;x++)
{
neuron=new Neuron;
layer->push_back(neuron);
if((layerNumber>1 && x>0) || layerNumber==layers.size())
{
for(int ss=0;ss<numberOfNeuronsOnPreviousLayer;ss++)
{
dendrite=new Dendrite;
dendrite->setInputNeuron(neuronsOnPreviousLayer[ss]);
neuron->addDendrite(dendrite);
}
}
}
if(neuronsOnPreviousLayer) delete [] neuronsOnPreviousLayer;
if(layerNumber==layers.size())
{
this->labels.resize(numberOfNeurons);
this->labels.insert(this->labels.begin(),numberOfNeurons,0.0);
}
}
void setInput(int neuronIndex,double input)
{
Neuron *neuron=getNeuron(1,neuronIndex);
neuron->setOutput(input);
}

void setLabel(int neuronIndex,double value)
{
list<double>::iterator labelIterator=labels.begin();
advance(labelIterator,neuronIndex);
*labelIterator=value;
}
private:
Neuron * getNeuron(int layerNumber,int neuronIndex)
{
list<list<Neuron *> *>::iterator layerIterator=layers.begin();
advance(layerIterator,layerNumber-1);
list<Neuron *> *layer=*(layerIterator);
list<Neuron *>::iterator neuronIterator=layer->begin();
advance(neuronIterator,neuronIndex);
return *neuronIterator;
}
public:
void setWeights(void (*ptr)(int,double *))
{ 
int numberOfDendritesInTheNetwork;
list<Neuron *> *layer;
Neuron *neuron;
numberOfDendritesInTheNetwork=0;
list<list<Neuron *> *>::iterator layersIterator=layers.begin();
++layersIterator;
while(layersIterator!=layers.end())
{
layer=*layersIterator;
list<Neuron *>::iterator layerIterator=layer->begin();
while(layerIterator!=layer->end())
{
neuron=*layerIterator;
numberOfDendritesInTheNetwork+=neuron->getDendritesCount();
++layerIterator;
}
++layersIterator;
}
double *dataSet=new double[numberOfDendritesInTheNetwork];
ptr(numberOfDendritesInTheNetwork,dataSet);
int ii=0;
layersIterator=layers.begin();
++layersIterator;
Dendrite *dendrite;
list<Dendrite *> *dendrites;
while(layersIterator!=layers.end())
{
layer=*layersIterator;
list<Neuron *>::iterator layerIterator=layer->begin();
while(layerIterator!=layer->end())
{
neuron=*layerIterator;
dendrites=neuron->getDendrites();
if(dendrites->size()>0)
{
list<Dendrite *>::iterator dendritesIterator=dendrites->begin();
while(dendritesIterator!=dendrites->end())
{
dendrite=*dendritesIterator;
dendrite->setWeight(dataSet[ii]);
ii++;
++dendritesIterator;
}
}
++layerIterator;
}
++layersIterator;
}
delete [] dataSet;
}
private :
feedForward()
{
int numberOfLayers=this->layers.size();
double sum=0.0;
list<list<Neuron *>*>::iterator layersIterator=this->layers.begin();
list<Neuron *>* currentLayer;
list<Neuron *>::iterator layerIterator;
Neuron * neuron;
Dendrite *dendrite;
list<Dendrite *>::iterator dendriteIterator;
list<Dendrite *>* dendrites;
while(layersIterator!=this->layers.end())
{
currentLayer=*layersIterator;
layerIterator=currentLayer->begin();
while(layerIterator!=currentLayer->end())
{
sum=0.0;
neuron=*layerIterator;
dendrites=neuron->getDendrites();
dendriteIterator=dendrites->begin();
while(dendriteIterator!=dendrites->end())
{
dendrite=*dendriteIterator;
sum+=(dendrite->getWeight()*dendrite->getInputNeuron()->getOutput());
dendriteIterator++;
}
if(dendrites->size()>0) neuron->setOutput(activationFunction(sum));
++layerIterator;
}
++layersIterator;
}
}	// end of feedForward() function

double computeError()
{
list<list<Neuron *>*>::reverse_iterator ri=layers.rbegin();
list<Neuron *> *currentLayer=*ri;
list<Neuron *>::iterator layerIterator=currentLayer->begin();
Neuron *currentNeuron;
double networkError;
double sum=0;
double error;
list<double>::iterator labelsIterator=labels.begin();
while(layerIterator!=currentLayer->end())
{
currentNeuron=*layerIterator;
error=(*labelsIterator)-(currentNeuron->getOutput());
currentNeuron->setError(error);
sum+=(1/2)*(error*error);
labelsIterator++;
layerIterator++;
}
networkError=sum;

}


public:
void train()
{
cout<<"train chala";
feedForward();
}
};

void weightGenerator(int size,double *dataSet)
{ 
std::default_random_engine generator;
std::normal_distribution<double> distribution(0.0,1.0);
for(int i=0;i<size;++i)
{
dataSet[i]=distribution(generator);
}
}

double af(double num)
{
cout<<"\naf chala";
return 1/(1+exp((-1)*num));
}

int main()
{
Network network(af);
network.setNumberOfLayers(4);
network.setNumberOfNeurons(1,2); // we are referring first layer with 1
network.setNumberOfNeurons(2,3);
network.setNumberOfNeurons(3,3);
network.setNumberOfNeurons(4,2);
network.setInput(0,0.0);
network.setInput(1,0.0);
network.setLabel(0,0.0);
network.setLabel(1,0.0);
network.setWeights(weightGenerator);
network.train();
/*network.setInput(0,1.0);
network.setInput(1,1.0);
network.predict();
double a=network.getOutput(0);
double b=network.getOutput(1);*/
return 0;
}