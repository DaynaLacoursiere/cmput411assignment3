#include "animation.h"

using namespace std;



void animation::readAttFile(char* fileName)
{
  ifstream infile(fileName);
  if (!infile) {
    cerr << "Error: unable to open obj file: " << fileName << endl;
    exit(1);
  }
  string line;

  while (getline(infile, line)) {
    istringstream stin(line);
    int v, b;
    float w, wsum = 0;
    string token;
    char c;
    vector< pair< unsigned int, float> > row;
    pair< unsigned int, float> weight;

    stin >> v;

    while (wsum < 1) {
      stin >> token;
      stringstream ss(token);
      ss >> c >> b >> c >> w;
      wsum += w;
      weight.first = b;
      weight.second = w;
      row.push_back(weight);
    }
    attachments.push_back(row);
  }

  infile.close();
}

void animation::writeAttFile(char* fileName)
{  
  ofstream outfile(fileName);

  for(size_t i = 0; i < attachments.size(); ++i)
  {
    outfile << i + 1 << " ";
    for(size_t j = 0; j < attachments[i].size(); ++j)
    {
      outfile << "(" << attachments[i][j].first << "," << attachments[i][j].second << ") ";
    }
    outfile << endl;
  }
  outfile.close();
}
