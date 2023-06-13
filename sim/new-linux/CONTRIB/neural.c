#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include "../SRC/types.h"
#include "../SRC/include.h"
#include "neural.h"

#define WEIGHT_MASK 31

static double WEIGHT[WEIGHT_MASK+1]=
{  1,  -1,   0, 0.9,  0.5,  100,  -50,    51,
 1.2, -0.8, 0.55, 1.1, 0.45, 10,   -3,    -5,
 0.3,-0.5, 0.2,  0.4,  0.6, 0.7,  0.8,  -0.7,
   3,   5, 999, -999,  9.1,-9.1,    2,  -1.5};


void InitNeuralNetwork(struct NeuralNetwork *net)
{
  net->InputNeuron      = NULL;
  net->HiddenNeuron     = NULL;
  net->OutputNeuron     = NULL;
  net->NumberOfNeurons  = 0;
  net->NumberOfSynapses = 0;
}

void ReachOutput(struct Neuron *neuron)
{
  struct Synapse *synapse;

  neuron->Output = 0;
  synapse = neuron->Axon;
  while(synapse)
  {
    switch ((u_short)synapse->To->Output)
    {
      case 3:  neuron->Output = 3;
               break;
      case 2:  neuron->Output = 3;
               synapse->To->Output = 3;
               break;
      case 1:  ReachOutput(synapse->To);
               if (synapse->To->Output == 3) neuron->Output = 3;
               break;
    }
    synapse = synapse->Next;
  }
}

long int NeuronId(struct Neuron *neuron,struct Neuron **neuron_a)
{
  long int i=0;

  if (neuron) while (neuron_a[i++] != neuron);
  return(i);
}

long int SynapseId(struct Synapse *synapse,struct Synapse **synapse_a)
{
  long int i=0;

  if (synapse) while (synapse_a[i++] != synapse);
  return(i);
}

void PrintNeuronId(struct NeuralNetwork *net,struct Neuron *neuron)
{
  struct Neuron *search;
  char          buffer[8];
  long int      i;

  strcpy(buffer,"X");
  search = net->InputNeuron;
  i = 0;
  while(search)
  {
    i++;
    if (search == neuron) sprintf(buffer,"I%d",i);
    search = search->Next;
  }
  search = net->HiddenNeuron;
  i = 0;
  while(search)
  {
    i++;
    if (search == neuron) sprintf(buffer,"H%d",i);
    search = search->Next;
  }
  search = net->OutputNeuron;
  i = 0;
  while(search)
  {
    i++;
    if (search == neuron) sprintf(buffer,"O%d",i);
    search = search->Next;
  }
  printf("%s",buffer);
}

void PrintNeuralNetwork(struct NeuralNetwork *net)
{
  struct Neuron  *neuron;
  struct Synapse *synapse;
  long int i,j;

  printf("Neural Network Structure:\n\n");
  i = 0;
  j = 0;
  neuron = net->InputNeuron;
  while(neuron)
  {
    i++;
    printf("I%d: ",i);
    synapse = neuron->Axon;
    while(synapse)
    {
      printf("\t");
      PrintNeuronId(net,synapse->To);
      printf("(%lg) ",synapse->Weight);
      synapse = synapse->Next;
    }
    printf("\n");
    neuron = neuron->Next;
  }
  i = 0;
  j = 0;
  neuron = net->HiddenNeuron;
  while(neuron)
  {
    i++;
    printf("H%d: ",i);
    synapse = neuron->Axon;
    while(synapse)
    {
      printf("\t");
      PrintNeuronId(net,synapse->To);
      printf("(%lg) ",synapse->Weight);
      synapse = synapse->Next;
    }
    printf("\n");
    neuron = neuron->Next;
  }
  i = 0;
  j = 0;
  neuron = net->OutputNeuron;
  while(neuron)
  {
    i++;
    printf("O%d: ",i);
    synapse = neuron->Axon;
    while(synapse)
    {
      printf("\t");
      PrintNeuronId(net,synapse->To);
      printf("(%lg) ",synapse->Weight);
      synapse = synapse->Next;
    }
    printf("\n");
    neuron = neuron->Next;
  }
}

struct Neuron *CreateNeuron(long int n)
{
  struct Neuron *neuron,*previous;

  if (n > 0)
  {
    previous = NULL;
    while(n--)
    {
      neuron         = (struct Neuron *)malloc(sizeof(struct Neuron));
      neuron->Output = 0.0;
      neuron->Sum    = 0.0;
      neuron->Axon   = NULL;
      neuron->Next   = previous;
      previous = neuron;
    }
    return(neuron);
  }
  else return((struct Neuron *)NULL);
}

void CreateSynapse(struct Neuron **from_l,long int from,struct Neuron **to_l,
                   long int to,long int t)
{
  struct Synapse *synapse;
  struct Neuron  *from_n,*to_n;

  from_n = *from_l;
  to_n   = *to_l;
  while(from--) from_n = from_n->Next;
  while(to--)   to_n   = to_n->Next;

  synapse = from_n->Axon;
  if (synapse)
  {
    while(synapse->Next) synapse = synapse->Next;
    synapse->Next = (struct Synapse *)malloc(sizeof(struct Synapse));
    synapse = synapse->Next;
  }
  else
  {
    from_n->Axon = (struct Synapse *)malloc(sizeof(struct Synapse));
    synapse = from_n->Axon;
  }

  synapse->To     = to_n;
  synapse->Type   = t;
  synapse->Next   = NULL;
}

void FreeAxon(struct Synapse *axon)
{
  struct Synapse *next;

  while(axon)
  {
    next = axon->Next;
    free(axon);
    // axon = axon->Next;
    axon = next;	// changed by RS
  }
}

long int CountNeuron(struct Neuron *neuron)
{
  long int n=0;

  while(neuron)
  {
    n++;
    neuron = neuron->Next;
  }
  return(n);
}

long int CountSynapse(struct Neuron *neuron)
{
  struct Synapse *synapse;
  long int       n=0;

  while(neuron)
  {
    synapse = neuron->Axon;
    while(synapse)
    {
      n++;
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  return(n);
}

void UpdateNumberOf(struct NeuralNetwork *net)
{
  net->NumberOfSynapses = CountSynapse(net->InputNeuron)
                        + CountSynapse(net->HiddenNeuron)
                        + CountSynapse(net->OutputNeuron);
  net->NumberOfNeurons  = CountNeuron(net->InputNeuron)
                        + CountNeuron(net->HiddenNeuron)
                        + CountNeuron(net->OutputNeuron);
}

void UpdateWeights(struct Neuron *neuron)
{
  struct Synapse *synapse;

  while(neuron)
  {
    synapse = neuron->Axon;
    while(synapse)
    {
      synapse->Weight = WEIGHT[synapse->Type & WEIGHT_MASK];
      synapse = synapse->Next;
    }
    neuron = neuron->Next;  
  }
}

void UpdateCoordinates(struct NeuralNetwork *net)
{
  struct Neuron *neuron;
  int           i,j,n,m,l;

  neuron = net->InputNeuron; i = 0;
  while(neuron)
  {
    neuron->x = (u_short)(INFO_WIDTH/2-(N_INPUT_NEURON-2*i-1)*INFO_WIDTH/
                (2.2*N_INPUT_NEURON));
    neuron->y = (u_short)40;
    neuron = neuron->Next; i++;
  }
  neuron = net->HiddenNeuron; i = 0;
  n = CountNeuron(neuron);
  while(neuron)
  {
    l = (n / 17) + 1;
    j = i % 17;
    if (i/17 == l-1) m = n % 17; else m = 17;
    neuron->x = (u_short)(INFO_WIDTH/2-(m-2*j-1)*INFO_WIDTH/(2.2*m));
    neuron->y = (u_short)(70 + 100/l + (i/17)*(200/l));
    neuron = neuron->Next; i++;
  }
  neuron = net->OutputNeuron; i=0;
  while(neuron)
  {
    neuron->x = (u_short)(INFO_WIDTH/2-(N_OUTPUT_NEURON-2*i-1)*INFO_WIDTH/
                (2.2*N_OUTPUT_NEURON));
    neuron->y = (u_short)296;
    neuron = neuron->Next; i++;
  }
}

void UpdateNeuralNetwork(struct NeuralNetwork *net)
{
  UpdateNumberOf(net);
  UpdateWeights(net->InputNeuron);
  UpdateWeights(net->HiddenNeuron);
  UpdateWeights(net->OutputNeuron);
  UpdateCoordinates(net);
}

void FreeNeuralNetwork(struct NeuralNetwork *net)
{
  struct Neuron *neuron,*next;

  if (net)
  {
    neuron = net->InputNeuron;
    while(neuron)
    {
      FreeAxon(neuron->Axon);
      next = neuron->Next;
      free(neuron);
      neuron = next;
    }
    neuron = net->HiddenNeuron;
    while(neuron)
    {
      FreeAxon(neuron->Axon);
      next = neuron->Next;
      free(neuron);
      neuron = next;
    }
    neuron = net->OutputNeuron;
    while(neuron)
    {
      FreeAxon(neuron->Axon);
      next = neuron->Next;
      free(neuron);
      neuron = next;
    }
  }
}

void WriteNeuralNetworkToFile(struct NeuralNetwork *network,FILE *file)
{
  long int       n_input,n_hidden,n_output,n_synapse,n_neuron,i,j,s,n;
  struct Neuron  **neuron_a,*neuron;
  struct Synapse **synapse_a,*synapse;

  n_input   = CountNeuron(network->InputNeuron);
  n_hidden  = CountNeuron(network->HiddenNeuron);
  n_output  = CountNeuron(network->OutputNeuron);
  n_neuron  = n_input + n_hidden + n_output;
  n_synapse = CountSynapse(network->InputNeuron)  + 
              CountSynapse(network->HiddenNeuron) +
              CountSynapse(network->OutputNeuron);
  neuron_a  = (struct Neuron  **)malloc(n_neuron *sizeof(struct Neuron  *));
  synapse_a = (struct Synapse **)malloc(n_synapse*sizeof(struct Synapse *));
  neuron = network->InputNeuron;
  j = 0;
  for(i=0;i<n_input;i++)
  {
    neuron_a[i] = neuron;
    synapse = neuron->Axon;
    while(synapse)
    {
      synapse_a[j++] = synapse;
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  neuron = network->HiddenNeuron;
  for(i=n_input;i<n_input+n_hidden;i++)
  {
    neuron_a[i] = neuron;
    synapse = neuron->Axon;
    while(synapse)
    {
      synapse_a[j++] = synapse;
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  neuron = network->OutputNeuron;
  for(i=n_input+n_hidden;i<n_neuron;i++)
  {
    neuron_a[i] = neuron;
    synapse = neuron->Axon;
    while(synapse)
    {
      synapse_a[j++] = synapse;
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  fprintf(file,"%ld,%ld,%ld,%ld\n",n_input,n_hidden,n_output,n_synapse);
  neuron = network->InputNeuron;
  for(i=0;i<n_input;i++)
  {
    s = SynapseId(neuron->Axon,synapse_a);
    n = NeuronId(neuron->Next,neuron_a);
    fprintf(file,"%hd,%lf,%ld,%ld\n",neuron->Type,neuron->Output,s,n);
    neuron=neuron->Next;
  }
  neuron = network->HiddenNeuron;
  for(i=n_input;i<n_input+n_hidden;i++)
  {
    s = SynapseId(neuron->Axon,synapse_a);
    n = NeuronId(neuron->Next,neuron_a);
    fprintf(file,"%hd,%lf,%ld,%ld\n",neuron->Type,neuron->Output,s,n);
    neuron=neuron->Next;
  }
  neuron = network->OutputNeuron;
  for(i=n_input+n_hidden;i<n_neuron;i++)
  {
    s = SynapseId(neuron->Axon,synapse_a);
    n = NeuronId(neuron->Next,neuron_a);
    fprintf(file,"%hd,%lf,%ld,%ld\n",neuron->Type,neuron->Output,s,n);
    neuron=neuron->Next;
  }
  neuron = network->InputNeuron;
  for(i=0;i<n_input;i++)
  {
    synapse = neuron->Axon;
    while(synapse)
    {
      n = NeuronId(synapse->To,neuron_a);
      s = SynapseId(synapse->Next,synapse_a);
      fprintf(file,"%hd,%lf,%ld,%ld\n",synapse->Type,synapse->Weight,n,s);
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  neuron = network->HiddenNeuron;
  for(i=n_input;i<n_input+n_hidden;i++)
  {
    synapse = neuron->Axon;
    while(synapse)
    {
      n = NeuronId(synapse->To,neuron_a);
      s = SynapseId(synapse->Next,synapse_a);
      fprintf(file,"%hd,%lf,%ld,%ld\n",synapse->Type,synapse->Weight,n,s);
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  neuron = network->OutputNeuron;
  for(i=n_input+n_hidden;i<n_neuron;i++)
  {
    synapse = neuron->Axon;
    while(synapse)
    {
      n = NeuronId(synapse->To,neuron_a);
      s = SynapseId(synapse->Next,synapse_a);
      fprintf(file,"%hd,%lf,%ld,%ld\n",synapse->Type,synapse->Weight,n,s);
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
  free(neuron_a);
  free(synapse_a);
}


void ReadNeuralNetworkFromFile(struct NeuralNetwork *network,FILE *file)
{
  short int            type;
  long int             i,j,n_synapse,n_input,n_hidden,n_output,n_neuron,v,v0;
  char                 *address;
  struct Neuron        *neuron,**neuron_a;
  struct Synapse       *synapse,*synapse_base,**synapse_a;
  double               fval;

  FreeNeuralNetwork(network);

  fscanf(file,"%ld,%ld,%ld,%ld\n",&n_input,&n_hidden,&n_output,&n_synapse);
  n_neuron = n_input + n_hidden + n_output;

  network->NumberOfSynapses = n_synapse;
  network->NumberOfNeurons  = n_neuron;

  neuron_a  = (struct Neuron  **)malloc(n_neuron *sizeof(struct Neuron  *));
  synapse_a = (struct Synapse **)malloc(n_synapse*sizeof(struct Synapse *));

  network->InputNeuron  = CreateNeuron(n_input);
  network->HiddenNeuron = CreateNeuron(n_hidden);
  network->OutputNeuron = CreateNeuron(n_output);

  synapse_base = (struct Synapse *)malloc(n_synapse*sizeof(struct Synapse));

  neuron = network->InputNeuron;
  for(i=0;i<n_input;i++)
  {
    neuron_a[i] = neuron;
    neuron = neuron->Next;
  }

  neuron = network->HiddenNeuron;
  for(i=n_input;i<n_input+n_hidden;i++)
  {
    neuron_a[i] = neuron;
    neuron = neuron->Next;
  }
  neuron = network->OutputNeuron;
  for(i=n_input+n_hidden;i<n_neuron;i++)
  {
    neuron_a[i] = neuron;
    neuron = neuron->Next;
  }
  synapse = synapse_base;
  for(i=0;i<n_synapse;i++)
  {
    synapse_a[i] = synapse;
    synapse++;
  }
  neuron = network->InputNeuron;
  for(i=0;i<n_input;i++)
  {
    fscanf(file,"%hd,%lf,%ld,%ld\n",&type,&fval,&v,&v0);
    neuron->Type = (u_char)type;
    neuron->Output = fval;
    if (v) neuron->Axon = synapse_a[v-1]; else neuron->Axon = NULL;
    if (v0) neuron->Next = neuron_a[v0-1]; else neuron->Next = NULL;
    neuron = neuron->Next;
  }
  neuron = network->HiddenNeuron;
  for(i=0;i<n_hidden;i++)
  {
    fscanf(file,"%hd,%lf,%ld,%ld\n",&type,&fval,&v,&v0);
    neuron->Type = (u_char)type;
    neuron->Output = fval;
    if (v) neuron->Axon = synapse_a[v-1]; else neuron->Axon = NULL;
    if (v0) neuron->Next = neuron_a[v0-1]; else neuron->Next = NULL;
    neuron = neuron->Next;
  }
  neuron = network->OutputNeuron;
  for(i=0;i<n_output;i++)
  {
    fscanf(file,"%hd,%lf,%ld,%ld\n",&type,&fval,&v,&v0);
    neuron->Type = (u_char)type;
    neuron->Output = fval;
    if (v) neuron->Axon = synapse_a[v-1]; else neuron->Axon = NULL;
    if (v0) neuron->Next = neuron_a[v0-1]; else neuron->Next = NULL;
    neuron = neuron->Next;
  }
  synapse = synapse_base;
  for(i=0;i<n_synapse;i++)
  {
    fscanf(file,"%hd,%lf,%ld,%ld\n",
           &type,&(synapse->Weight),&v,&v0);
    synapse->Type = (u_char)type;
    if (v) synapse->To = neuron_a[v-1]; else synapse->To = NULL;
    if (v0) synapse->Next = synapse_a[v0-1]; else synapse->Next = NULL;
    synapse++;
  }
  free(neuron_a);
  free(synapse_a);
  UpdateNeuralNetwork(network);
}


void CreateCrazyNeuralNetwork(struct NeuralNetwork *network)
{
  struct Neuron  **input,**hidden,**output;
  short int i,j;

  FreeNeuralNetwork(network);
  input   = &(network->InputNeuron);
  hidden  = &(network->HiddenNeuron);
  output  = &(network->OutputNeuron);  

  network->InputNeuron  = CreateNeuron(9);
  network->HiddenNeuron = CreateNeuron(3);
  network->OutputNeuron = CreateNeuron(2);

  CreateSynapse(input,0,output,0,20);
  CreateSynapse(input,0,output,1,20);
  CreateSynapse(input,3,output,0,7);
  CreateSynapse(input,4,output,1,7);
  CreateSynapse(input,2,output,0,11);
  CreateSynapse(input,5,output,1,11);

  CreateSynapse(input,4,hidden,1,2);
  CreateSynapse(hidden,2,hidden,1,1);
  CreateSynapse(hidden,0,hidden,1,4);
  CreateSynapse(hidden,1,output,1,3);

  CreateSynapse(output,1,hidden,0,5);

  UpdateNeuralNetwork(network);
}

short int WeightNb(double w)
{
  short int i;

  for(i=0;i<WEIGHT_MASK+1;i++) if (WEIGHT[i]==w) break;
  if (i == WEIGHT_MASK+1)
  {
    perror("Invalid weight in neural.network file");
    exit(1);
  }
  return(i);
}

void ReadNeuralNetwork(struct NeuralNetwork *network)
{
  struct Neuron  **input,**hidden,**output;
  long int n_input,n_hidden,n_output,i,j,k,w;
  FILE *file;
  char c;
  double weight=0.0;

  file = fopen("NEURAL/neural.network","r");
  if (file)
  {
    FreeNeuralNetwork(network);
    input   = &(network->InputNeuron);
    hidden  = &(network->HiddenNeuron);
    output  = &(network->OutputNeuron);  
    fscanf(file,"%ld,%ld,%ld\n",&n_input,&n_hidden,&n_output);
    network->InputNeuron  = CreateNeuron(n_input);
    network->HiddenNeuron = CreateNeuron(n_hidden);
    network->OutputNeuron = CreateNeuron(n_output);
    for(i=0; i<n_input;i++)
    {
      fscanf(file,"I%ld:",&j);
      /* printf("I%ld:",j); */
      while(TRUE)
      {
        fscanf(file,"%1c",&c);
        if (c=='\n') break;
        else if (c=='H')
        {
          fscanf(file,"%ld(%lf)",&k,&weight);
          CreateSynapse(input,j-1,hidden,k-1,WeightNb(weight));
          /* printf(" H%ld(%lg)",k,weight); */
        }
        else if (c=='O')
        {
          fscanf(file,"%ld(%lf)",&k,&weight);
          CreateSynapse(input,j-1,output,k-1,WeightNb(weight));
          /* printf(" O%ld(%lg)",k,weight); */
        }
      }
      /* printf("\n"); */
    }
    for(i=0; i<n_hidden;i++)
    {
      fscanf(file,"H%ld:",&j);
      /* printf("H%ld:",j); */
      while(TRUE)
      {
        fscanf(file,"%1c",&c);
        if (c=='\n') break;
        else if (c=='H')
        {
          fscanf(file,"%ld(%lf)",&k,&weight);
          CreateSynapse(hidden,j-1,hidden,k-1,WeightNb(weight));
          /* printf(" H%ld(%lg)",k,weight); */
        }
        else if (c=='O')
        {
          fscanf(file,"%ld(%lf)",&k,&weight);
          CreateSynapse(hidden,j-1,output,k-1,WeightNb(weight));
          /* printf(" O%ld(%lg)",k,weight); */
        }
      }
      /* printf("\n"); */
    }
    for(i=0; i<n_output;i++)
    {
      fscanf(file,"O%ld:",&j);
      /* printf("O%ld:",j); */
      while(TRUE)
      {
        fscanf(file,"%1c",&c);
        if (c=='\n') break;
        else if (c=='H')
        {
          fscanf(file,"%ld(%lf)",&k,&weight);
          CreateSynapse(output,j-1,hidden,k-1,WeightNb(weight));
          /* printf(" H%ld(%lg)",k,weight); */
        }
        else if (c=='O')
        {
          fscanf(file,"%ld(%lf)",&k,&weight);
          CreateSynapse(output,j-1,output,k-1,WeightNb(weight));
          /* printf(" O%ld(%lg)",k,weight); */
        }
      }
      /* printf("\n"); */
    }
    fclose(file);
    UpdateNeuralNetwork(network);
  }
  else WriteComment("unable to find neural.network file in NEURAL directory");
}

void ResetNeuralNetwork(struct NeuralNetwork *net)
{
  struct Neuron *neuron;

  neuron = net->InputNeuron;
  while(neuron)
  {
    neuron->Output = INHIBITED;
    neuron = neuron->Next;
  }
  neuron = net->HiddenNeuron;
  while(neuron)
  {
    neuron->Output = INHIBITED;
    neuron = neuron->Next;
  }
  neuron = net->OutputNeuron;
  while(neuron)
  {
    neuron->Output = INHIBITED;
    neuron = neuron->Next;
  }
}

void InitInputNeurons(struct Robot *robot,struct NeuralNetwork *network)
{
  struct Neuron *neuron;
  short int     motor,IR_sensor;

  neuron = network->InputNeuron;

  if (neuron)
  {neuron->Output = EXCITED;
   neuron = neuron->Next;
   if (neuron)
   {neuron->Output = (double)robot->IRSensor[0].DistanceValue / 1023;
    neuron = neuron->Next;
    if (neuron)
    {neuron->Output = (double)robot->IRSensor[1].DistanceValue / 1023;
     neuron = neuron->Next;
     if (neuron)
     {neuron->Output = (double)robot->IRSensor[2].DistanceValue / 1023;
      neuron = neuron->Next;
      if (neuron)
      {neuron->Output = (double)robot->IRSensor[3].DistanceValue / 1023;
       neuron = neuron->Next;
       if (neuron)
       {neuron->Output = (double)robot->IRSensor[4].DistanceValue / 1023;
        neuron = neuron->Next;
        if (neuron)
        {neuron->Output = (double)robot->IRSensor[5].DistanceValue / 1023;
         neuron = neuron->Next;
         if (neuron)
         {neuron->Output = (double)robot->IRSensor[6].DistanceValue / 1023;
          neuron = neuron->Next;
          if (neuron)
          {neuron->Output = (double)robot->IRSensor[7].DistanceValue / 1023;
           neuron = neuron->Next;
           if (neuron)
           {neuron->Output = EXCITED 
                           - (double)robot->IRSensor[0].LightValue / 525;
            neuron = neuron->Next;
            if (neuron)
            {neuron->Output = EXCITED
                            - (double)robot->IRSensor[1].LightValue / 525;
             neuron = neuron->Next;
             if (neuron)
             {neuron->Output = EXCITED
                             - (double)robot->IRSensor[2].LightValue / 525;
              neuron = neuron->Next;
              if (neuron)
              {neuron->Output = EXCITED
                              - (double)robot->IRSensor[3].LightValue / 525;
               neuron = neuron->Next;
               if (neuron)
               {neuron->Output = EXCITED
                               - (double)robot->IRSensor[4].LightValue / 525;
                neuron = neuron->Next;
                if (neuron)
                {neuron->Output = EXCITED
                                - (double)robot->IRSensor[5].LightValue / 525;
                 neuron = neuron->Next;
                 if (neuron)
                 {neuron->Output = EXCITED
                                 - (double)robot->IRSensor[6].LightValue / 525;
                  neuron = neuron->Next;
                  if (neuron)
                  {neuron->Output = EXCITED
                                  -(double)robot->IRSensor[7].LightValue / 525;
                   neuron = neuron->Next;
                  }
                 }
                }
               }
              }
             }
            }
           }
	  }
         }
        }
       }
      }
     }
    }
   }
  }
}

void InitEffectors(struct Robot *robot,struct NeuralNetwork *network)
{
  struct Neuron *neuron;

  neuron            = network->OutputNeuron;
  if (neuron)
  {
    robot->Motor[1].Value = (short int)floor(0.5 + neuron->Output * 10.0);
    neuron                = neuron->Next;
    if (neuron)
    {
      robot->Motor[0].Value  = (short int)floor(0.5 + neuron->Output * 10.0);
      neuron                 = neuron->Next;
      if (neuron)
      {
        robot->Motor[1].Value -= (short int)floor(0.5 + neuron->Output * 10.0);
        neuron                 = neuron->Next;
        if (neuron)
        {
          robot->Motor[0].Value -= (short int)floor(0.5+neuron->Output * 10.0);
        }
      }
    }
  }
}

void Propagate(struct Neuron *neuron)
{
  struct Synapse *synapse,*syn;
  int i=0;

  while(neuron)
  {
    i++;
    synapse = neuron->Axon;
    while(synapse)
    {
      switch(synapse->Type & WEIGHT_MASK)
      {
        case 2:  synapse->Weight  += 0.05 * 
                                     synapse->To->Output * 
                                     (neuron->Output - synapse->Weight);
        default: synapse->To->Sum += synapse->Weight * neuron->Output;
	break;
      }
      synapse = synapse->Next;
    }
    neuron = neuron->Next;    
  }
}

void CalculateOutput(struct Neuron *neuron)
{
  while(neuron)
  {
    if (neuron->Sum > EXCITED) neuron->Sum = EXCITED;
    else if (neuron->Sum < 0.0) neuron->Sum = 0.0;
    neuron->Output = neuron->Sum;
    neuron->Sum = 0.0;
    neuron      = neuron->Next;
  }
}

void Think(struct NeuralNetwork *net)
{
  Propagate(net->InputNeuron);
  Propagate(net->HiddenNeuron);
  Propagate(net->OutputNeuron);
  CalculateOutput(net->HiddenNeuron);
  CalculateOutput(net->OutputNeuron);
}

double TestNeuralNetwork(struct NeuralNetwork *net)
{
  short int     n_i,u_i,n_h,u_h,n_o,u_o,sum;
  double        score;
  struct Neuron *neuron,*next;

  if ((net->InputNeuron)&&(net->OutputNeuron))
  {
    neuron = net->InputNeuron;
    while(neuron)
    {
      neuron->Output = 0;
      neuron = neuron->Next;
    }
    neuron = net->HiddenNeuron;
    while(neuron)
    {
      neuron->Output = 1;
      neuron = neuron->Next;
    }
    neuron = net->OutputNeuron;
    while(neuron)
    {
      neuron->Output = 2;
      neuron = neuron->Next;
    }
    neuron = net->InputNeuron;
    while(neuron)
    {
      ReachOutput(neuron);
      neuron = neuron->Next;
    }
    n_i = 0;
    u_i = 0;
    neuron = net->InputNeuron;
    while(neuron)
    {
      n_i++;
      next = neuron->Next;
      if (neuron->Output == 3) u_i++;
      neuron = next;
    }
    n_h = 0;
    u_h = 0;
    neuron = net->HiddenNeuron;
    while(neuron)
    {
      n_h++;
      if (neuron->Output == 3) u_h++;
      neuron = neuron->Next;
    }
    n_o = 0;
    u_o = 0;
    neuron = net->OutputNeuron;
    while(neuron)
    {
      n_o++;
      if (neuron->Output == 3) u_o++;
      neuron = neuron->Next;
    }
    sum = n_i + n_o + n_h;
    if (sum) score = (100.0 * (u_i + u_o + u_h))/sum; else score = 0.0;
    return(score);
  }
  else return(0.0);
}

void DrawInputActivities(struct Neuron *neuron)
{
  int i;

  for(i=0;i<N_INPUT_NEURON;i++)
  {
    if (neuron)
    {
      Color(BLUE + (char)floor(neuron->Output * 8.99999999));
      neuron = neuron->Next;
    }
    else Color(GREY);
    FillArc((int)(INFO_WIDTH/2-(N_INPUT_NEURON-2*i-1)*INFO_WIDTH/
            (2.2*N_INPUT_NEURON)-5),35,11,11,0,360*64);
  }
}

void DrawHiddenActivities(struct Neuron *neuron)
{
  while(neuron)
  {
    Color(BLUE + (char)floor(neuron->Output * 8.99999999));
    FillArc(neuron->x-5,neuron->y-5,11,11,0,360*64);
    neuron = neuron->Next;
  }
}

void DrawOutputActivities(struct Neuron *neuron)
{
  int i;

  for(i=0;i<N_OUTPUT_NEURON;i++)
  {
    if (neuron)
    {
      Color(BLUE + (char)floor(neuron->Output * 8.99999999));
      neuron = neuron->Next;
    }
    else Color(GREY);
    FillArc((int)(INFO_WIDTH/2-(N_OUTPUT_NEURON-2*i-1)*INFO_WIDTH/
            (2.2*N_OUTPUT_NEURON)-5),291,11,11,0,360*64);
  }
}

void DrawInputLayer()
{
  int i;

  Color(WHITE);
  for(i=0;i<N_INPUT_NEURON;i++)
   FillArc((int)(INFO_WIDTH/2-(N_INPUT_NEURON-2*i-1)*INFO_WIDTH/
           (2.2*N_INPUT_NEURON)-6),34,13,13,40*64,220*64);
  Color(BLACK);
  for(i=0;i<N_INPUT_NEURON;i++)
  {
    FillArc((int)(INFO_WIDTH/2-(N_INPUT_NEURON-2*i-1)*INFO_WIDTH/
            (2.2*N_INPUT_NEURON)-6),34,13,13,0,40*64);
    FillArc((int)(INFO_WIDTH/2-(N_INPUT_NEURON-2*i-1)*INFO_WIDTH/
            (2.2*N_INPUT_NEURON)-6),34,13,13,0,-140*64);
  }
}

void DrawHiddenLayer(struct Neuron *neuron)
{
  while(neuron)
  {
    Color(WHITE);
    FillArc(neuron->x-6,neuron->y-6,13,13,40*64,220*64);
    Color(BLACK);
    FillArc(neuron->x-6,neuron->y-6,13,13,0,40*64);
    FillArc(neuron->x-6,neuron->y-6,13,13,0,-140*64);
    neuron = neuron->Next;
  }
}

void DrawOutputLayer()
{
  int i;

  Color(WHITE);
  for(i=0;i<N_OUTPUT_NEURON;i++)
   FillArc((int)(INFO_WIDTH/2-(N_OUTPUT_NEURON-2*i-1)*INFO_WIDTH/
           (2.2*N_OUTPUT_NEURON)-6),290,13,13,40*64,220*64);
  Color(BLACK);
  for(i=0;i<N_OUTPUT_NEURON;i++)
  {
    FillArc((int)(INFO_WIDTH/2-(N_OUTPUT_NEURON-2*i-1)*INFO_WIDTH/
            (2.2*N_OUTPUT_NEURON)-6),290,13,13,0,40*64);
    FillArc((int)(INFO_WIDTH/2-(N_OUTPUT_NEURON-2*i-1)*INFO_WIDTH/
            (2.2*N_OUTPUT_NEURON)-6),290,13,13,0,-140*64);
  }
}

void DrawNeuralStructure(struct Neuron *neuron)
{
  struct Synapse *synapse;
  int            d,dx,dy,x1,y1,x2,y2;
  XPoint         polygon[3];

  while(neuron)
  {
    synapse = neuron->Axon;
    while(synapse)
    {
      Color(BLUE + synapse->Type & WEIGHT_MASK);
      if (neuron == synapse->To)
       DrawArc(neuron->x,neuron->y,13,13,0,360*64);
      else
      {
        x1 = neuron->x;
        y1 = neuron->y;
        x2 = synapse->To->x;
        y2 = synapse->To->y;
        DrawLine(x1,y1,x2,y2);
        dx = x2 - x1;
        dy = y2 - y1;
        d = (int)hypot((double)dx,(double)dy);
        polygon[0].x = (x1+x2)/2 + X_O;
        polygon[0].y = (y1+y2)/2 + Y_O;
        polygon[1].x = polygon[0].x - (dx * 10 + dy * 5)/d;
        polygon[1].y = polygon[0].y - (dy * 10 - dx * 5)/d;
        polygon[2].x = polygon[0].x - (dx * 10 - dy * 5)/d;
        polygon[2].y = polygon[0].y - (dy * 10 + dx * 5)/d;
        XFillPolygon(display,window,gc,polygon,3,Convex,CoordModeOrigin);
      }
      synapse = synapse->Next;
    }
    neuron = neuron->Next;
  }
}

void DrawNeuralNetwork(struct NeuralNetwork *net)
{
  Color(BLACK);
  DrawText(INFO_WIDTH/2-44,26,"input layer");
  DrawText(INFO_WIDTH/2-48,324,"output layer");
  DrawNeuralStructure(net->InputNeuron);
  DrawNeuralStructure(net->HiddenNeuron);
  DrawNeuralStructure(net->OutputNeuron);
  DrawInputLayer();
  DrawHiddenLayer(net->HiddenNeuron);
  DrawOutputLayer();
  DrawInputActivities(net->InputNeuron);
  DrawHiddenActivities(net->HiddenNeuron);
  DrawOutputActivities(net->OutputNeuron);
}

void DrawNeuralLegend()
{
  int            i;
  int            d,dx,dy,x1,y1,x2,y2;
  XPoint         polygon[3];
  char           text[32];

  Color(BLACK);
  DrawText(INFO_WIDTH/2-80,20,"Synaptic Connections");
  for(i=0;i<WEIGHT_MASK+1;i++)
  {
    Color(BLUE + i);
    x1 = 20 + (INFO_WIDTH/4) * (i / 8);
    y1 = 60 + (i % 8) * 32;
    x2 = 40 + (INFO_WIDTH/4) * (i / 8);
    y2 = 60 + (i % 8) * 32;
    DrawLine(x1,y1,x2,y2);
    dx = x2 - x1;
    dy = y2 - y1;
    d = (int)hypot((double)dx,(double)dy);
    polygon[0].x = (x1+x2)/2 + X_O + 10;
    polygon[0].y = (y1+y2)/2 + Y_O;
    polygon[1].x = polygon[0].x - (dx * 10 + dy * 5)/d;
    polygon[1].y = polygon[0].y - (dy * 10 - dx * 5)/d;
    polygon[2].x = polygon[0].x - (dx * 10 - dy * 5)/d;
    polygon[2].y = polygon[0].y - (dy * 10 + dx * 5)/d;
    XFillPolygon(display,window,gc,polygon,3,Convex,CoordModeOrigin);
    Color(BLACK);
    sprintf(text,"%lg",WEIGHT[i]);
    DrawText(x2+24-strlen(text)*4,y2+4,text);
  }
}
