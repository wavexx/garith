#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ufm.h>

using std::cerr;
using std::cout;

const float mul = 1000.;


int
main(int argc, char* argv[])
{
  if(argc != 3)
  {
    cerr << argv[0] << " usage: " << argv[0] << " font prec\n";
    return 2;
  }

  char* font = argv[1];
  float prec = strtod(argv[2], NULL);

  unsigned int h;
  if(ufmOpenTypeface(font, &h) != UFM_NO_ERROR)
  {
    cerr << argv[0] << ": cannot open typeface\n";
    return 1;
  }

  unsigned int n;
  ufmVectorOutline* o;
  if(ufmGetVectorOutline(1, &h, UFM_BYTE1, 0, NULL, prec, &n, &o) !=
      UFM_NO_ERROR)
  {
    cerr << argv[0] << ": error getting the outlines\n";
    return 1;
  }

  // header
  cout << "af 1 " << mul << std::endl;

  for(unsigned int i = 0; i != n; ++i)
  {
    // only grab the ascii subset
    if(o[i].charCode.signedint < 0 || o[i].charCode.signedint > 126)
      continue;

    // static info
    cout << static_cast<char>(o[i].charCode.signedint) << " " <<
      (mul * o[i].xMoveScalable) << " " << o[i].nPaths;

    // paths
    int rv = 0;
    for(unsigned int p = 0; p != o[i].nPaths; ++p)
    {
      cout << " " << o[i].nPoints[p];
      for(unsigned int v = 0; v != o[i].nPoints[p]; ++v, ++rv)
	cout << " " << (mul * o[i].points[rv].x) << " " <<
	  (mul * o[i].points[rv].y);
    }

    cout << std::endl;
  }

  return 0;
}
