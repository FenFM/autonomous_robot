#ifndef NEURAL_H
#define NEURAL_H 

#define N_INPUT_NEURON           17
#define N_OUTPUT_NEURON          4
#define EXCITED                  1.0
#define INHIBITED                0

struct Synapse
{
  u_char             Type;
  double             Weight;
  struct Neuron      *To;
  struct Synapse     *Next;
};

struct Neuron
{
  u_char             Type;
  u_short            x,y;
  double             Output;
  double             Sum;
  struct Synapse     *Axon;
  struct Neuron      *Next;
};

struct NeuralNetwork
{
  u_long             NumberOfSynapses;
  u_long             NumberOfNeurons;
  struct Neuron      *InputNeuron;
  struct Neuron      *HiddenNeuron;
  struct Neuron      *OutputNeuron;
};

#endif
