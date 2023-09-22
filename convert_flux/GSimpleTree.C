#define GSimpleTree_cxx
#include "GSimpleTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;


class Vector3D {
public:
   Vector3D(double x, double y, double z) {
      this->x = x;
      this->y = y;
      this->z = z;
   }
 
   double dot(const Vector3D& rhs) const {
      return x * rhs.x + y * rhs.y + z * rhs.z;
   }
 
   Vector3D operator-(const Vector3D& rhs) const {
      return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
   }
 
   Vector3D operator*(double rhs) const {
      return Vector3D(rhs*x, rhs*y, rhs*z);
   }
 
   friend std::ostream& operator<<(std::ostream&, const Vector3D&);
 
   double x, y, z;
};


std::ostream & operator<<(std::ostream & os, const Vector3D &f) {
   std::stringstream ss;
   ss << "(" << f.x << ", " << f.y << ", " << f.z << ")";
   return os << ss.str();
}

Vector3D intersectPoint(Vector3D rayVector, Vector3D rayPoint, Vector3D planeNormal, Vector3D planePoint) {
   Vector3D diff = rayPoint - planePoint;
   double prod1 = diff.dot(planeNormal);
   double prod2 = rayVector.dot(planeNormal);
   double prod3 = prod1 / prod2;
   return rayPoint - rayVector * prod3;
}




void GSimpleTree::Loop()
{
//   In a ROOT session, you can do:
//      root> .L GSimpleTree.C
//      root> GSimpleTree t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;


   TFile *outfile = new TFile("uboone_flux.root", "RECREATE", "MicroBooNE Flux");
   TTree *outtree = new TTree("h3002", "MicroBooNE Flux");
   Float_t Enu, nnu[3], xnu, ynu, znu, norm;
   Int_t mode;
   outtree->Branch( "Enu", &Enu, "Enu/F" );
   outtree->Branch( "nnu", &nnu, "nnu[3]/F" );
   outtree->Branch( "xnu", &xnu, "xnu/F" );
   outtree->Branch( "ynu", &ynu, "ynu/F" );
   outtree->Branch( "znu", &znu, "znu/F" );
   outtree->Branch( "mode", &mode, "mode/I" );
   outtree->Branch( "norm", &norm, "norm/F" );


   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // if (jentry % 100000) cout << "At entry " << jentry << " of " << nentries << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      double p = std::sqrt(px * px + py * py + pz * pz);
      if (p == 0) continue;
            
      Vector3D rv = Vector3D(px/p, py/p, pz/p); // The ray vector (the direction of the neutrino)
      Vector3D rp = Vector3D(vtxx, vtxy, vtxz); // The ray point (starting point of the neutrino)
      Vector3D pn = Vector3D(0.0, 0.0, 1.0);    // The normal to the plane we want to project to
      Vector3D pp = Vector3D(0.0, 0.0, 0.0);    // A point on the plane (here find the intersection with a plane at z=0)
      Vector3D ip = intersectPoint(rv, rp, pn, pp);

      // std::cout << "The ray intersects the plane at " << ip << std::endl;
 
      Double_t nuwro_pdg = 0;
      if (pdg == 14) nuwro_pdg = 11;
      if (pdg == -14) nuwro_pdg = 21;
      if (pdg == 12) nuwro_pdg = 31;
      if (pdg == -12) nuwro_pdg = 41;

      mode = nuwro_pdg;
      Enu = E; // GeV 
      xnu = vtxx * 100; // from m to cm, as NuWro expects cm
      ynu = vtxy * 100; // from m to cm, as NuWro expects cm
      znu = vtxz * 100; // from m to cm, as NuWro expects cm

      nnu[0] = px/p; // unit vector components 
      nnu[1] = py/p; // unit vector components 
      nnu[2] = pz/p; // unit vector components 
      norm = 1;

      outtree->Fill();

   }

   outfile->Write();
}









