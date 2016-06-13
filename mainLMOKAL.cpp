// #include "functions.h"
#include <fftw3.h> 
#include "../Moka/cosmology.h"
#include "../Moka/fftw_lens.h"
#include <CCfits/CCfits>

using namespace CCfits;

void readFits (std::string fn, std:: valarray<float> &map, int &nx, int &ny){
  std::auto_ptr<FITS> ff(new FITS (fn, Read));
  PHDU *h0=&ff->pHDU();
  nx=h0->axis(0);
  ny=h0->axis(1);
  h0->read(map);
}

void writeFits(std:: string filename,std:: valarray<float> f, int npix, int npixy){
  long naxis=2;
  long naxes[2]={npix,npixy};
  std::auto_ptr<FITS> fout(new FITS(filename,FLOAT_IMG,naxis,naxes));
  std::vector<long> naxex(2);
  naxex[0]=npix;
  naxex[1]=npixy;
  PHDU *phout=&fout->pHDU();
  phout->write( 1, npix*npixy, f );
}

int main(){
  double omegaM;
  double omegaL;
  double h;
  double zlens;
  double xarcsec;
  std:: string filekappa;
  
  std:: cin >> omegaM;
  std:: cin >> omegaL;
  std:: cin >> h;
  std:: cin >> zlens;
  std:: cin >> xarcsec;
  std:: cin >> filekappa;  
  
  //************** C O S M O L O G Y **********************//
  cosmology co(omegaM,omegaL,h);
  //*******************************************************//
    
  std:: cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std:: endl;

  std:: cout << "   running assuming the follwing input parameters " << std:: endl;
  std:: cout << "      omegaM = " << omegaM << std:: endl;
  std:: cout << "      omegaL = " << omegaL << std:: endl;
  std:: cout << "           h = " << h << std:: endl;
  std:: cout << "       zlens = " << zlens << std:: endl;
  std:: cout << "   arcsec x  = " << xarcsec << std:: endl;    
  std:: cout << "   kappa map = " << filekappa << std:: endl;

  std:: valarray<float> map;
  int nx,ny;
  readFits(filekappa,map,nx,ny);
  double Dl = co.angularDist(0.,zlens)*co.cn.lightspeed*1.e-7;
  double boxl = xarcsec/3600./180.*M_PI*Dl;

  // you may increase this vaule to reduce the effect of "mirror clusters" caused going in the Fourier Space!
  int zerosize=2;
  int npix_f=0;
  fftw_lens flens(map,nx,boxl,zerosize,npix_f); 
  flens.lensingComponents();    
  // get out the shear components
  double *sg1d;
  double *sg2d;
  sg1d=flens.shear1();
  sg2d=flens.shear2();

  int nnx = zerosize*nx;
  int nny = zerosize*ny;
  
  std::valarray<float> g1(nx*ny),g2(nx*ny),g(nx*ny);
  for( int j=nny/2-ny/2; j<nny/2+ny/2; j++ )
    for( int i=nnx/2-nx/2; i<nnx/2+nx/2; i++ ){
      int ii = i-int(nnx/2-nx/2); 
      int jj = j-int(nny/2-ny/2); 
      
      g1[ii+nx*jj] = float(sg1d[i+nnx*j]/nnx/nny);
      g2[ii+nx*jj] = float(-sg2d[i+nnx*j]/nnx/nny);
      g[ii+nx*jj] = sqrt(gsl_pow_2(g1[ii+nx*jj]) + gsl_pow_2(g2[ii+nx*jj]));
    }
  
  std:: string filegamma = filekappa + "_gamma.fits";
  // check if the file exist ... if it does remove it!
  std:: ifstream ifile;
  ifile.open(filegamma.c_str());
  if(ifile.is_open()){
    std:: cout << " file with gamma info exist I remove it " << std:: endl;
    std:: cout << " and create a new one " << std:: endl;
    remove(filegamma.c_str());
  }
  writeFits(filegamma,g,nx,ny);
  
  std:: cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std:: endl;
  std:: cout << " ... end of work! ;-) " << std:: endl;
  
  return 0;
}
