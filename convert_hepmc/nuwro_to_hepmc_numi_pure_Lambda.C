#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "TTree.h"
#include "TFile.h"
#include "FV.h"



void nuwro_to_hepmc_numi_pure_Lambda(std::string indir , std::string nuwro_file_name , std::string outdir, int eventsperfile=100) {

   std::string cmd = "mkdir -p " + outdir;
   gSystem->Exec(cmd.c_str());


   // Input file
   std::string infile = indir + nuwro_file_name + ".root";
   TFile* f = new TFile(infile.c_str());
   TTree* t = (TTree*)f->Get("treeout");

   //number of events converted
   int event_count=0;


   t->SetMakeClass(1); // tree in MakeClass mode
   t->SetBranchStatus("*", 0); // disable all branches

   Bool_t          flag_qel;
   Bool_t          flag_res;
   Bool_t          flag_dis;
   Bool_t          flag_coh;
   Bool_t          flag_mec;
   Bool_t          flag_hyp;
   Bool_t          flag_cc;

   static constexpr Int_t kMaxin = 100;
   static constexpr Int_t kMaxtemp = 100;
   static constexpr Int_t kMaxout = 100;
   static constexpr Int_t kMaxpost = 100;
   static constexpr Int_t kMaxall = 100;


   t->SetBranchStatus("flag.cc",1);
   t->SetBranchAddress("flag.cc", &flag_cc);

   t->SetBranchStatus("flag.qel",1);
   t->SetBranchAddress("flag.qel", &flag_qel);
   t->SetBranchStatus("flag.res",1);
   t->SetBranchAddress("flag.res", &flag_res);
   t->SetBranchStatus("flag.dis",1);
   t->SetBranchAddress("flag.dis", &flag_dis);
   t->SetBranchStatus("flag.coh",1);
   t->SetBranchAddress("flag.coh", &flag_coh);
   t->SetBranchStatus("flag.mec",1);
   t->SetBranchAddress("flag.mec", &flag_mec);
   t->SetBranchStatus("flag.hyp",1);
   t->SetBranchAddress("flag.hyp", &flag_hyp);

   Int_t in_;

   Double_t        in_t[kMaxin];   //[in_]
   Double_t        in_x[kMaxin];   //[in_]
   Double_t        in_y[kMaxin];   //[in_]
   Double_t        in_z[kMaxin];   //[in_]
   Double_t        in__mass[kMaxin];   //[in_]
   Int_t           in_pdg[kMaxin];   //[in_]
   Double_t        in_r_x[kMaxin];   //[in_]
   Double_t        in_r_y[kMaxin];   //[in_]
   Double_t        in_r_z[kMaxin];   //[in_]

   t->SetBranchStatus("in", 1); // activate branch
   t->SetBranchAddress("in", &in_);
   t->SetBranchStatus("in.t",1);
   t->SetBranchAddress("in.t", in_t);
   t->SetBranchStatus("in.x",1);
   t->SetBranchAddress("in.x", in_x);
   t->SetBranchStatus("in.y",1);
   t->SetBranchAddress("in.y", in_y);
   t->SetBranchStatus("in.z",1);
   t->SetBranchAddress("in.z", in_z);
   t->SetBranchStatus("in._mass", 1);
   t->SetBranchAddress("in._mass", in__mass);
   t->SetBranchStatus("in.pdg", 1);
   t->SetBranchAddress("in.pdg", in_pdg);
   t->SetBranchStatus("in.r.x", 1);
   t->SetBranchAddress("in.r.x", in_r_x);
   t->SetBranchStatus("in.r.y", 1);
   t->SetBranchAddress("in.r.y", in_r_y);
   t->SetBranchStatus("in.r.z", 1);
   t->SetBranchAddress("in.r.z", in_r_z);


   Int_t           post_;

   Double_t        post_t[kMaxpost];   //[in_]
   Double_t        post_x[kMaxpost];   //[in_]
   Double_t        post_y[kMaxpost];   //[in_]
   Double_t        post_z[kMaxpost];   //[in_]
   Double_t        post__mass[kMaxpost];   //[in_]
   Int_t           post_pdg[kMaxpost];   //[in_]

   t->SetBranchStatus("post", 1); // activate branch
   t->SetBranchAddress("post", &post_);
   t->SetBranchStatus("post.t",1);
   t->SetBranchAddress("post.t", post_t);
   t->SetBranchStatus("post.x",1);
   t->SetBranchAddress("post.x", post_x);
   t->SetBranchStatus("post.y",1);
   t->SetBranchAddress("post.y", post_y);
   t->SetBranchStatus("post.z",1);
   t->SetBranchAddress("post.z", post_z);
   t->SetBranchStatus("post._mass", 1);
   t->SetBranchAddress("post._mass", post__mass);
   t->SetBranchStatus("post.pdg", 1);
   t->SetBranchAddress("post.pdg", post_pdg);


   Double_t        r_x;   
   Double_t        r_y;   
   Double_t        r_z;   

   t->SetBranchStatus("r.x", 1);
   t->SetBranchAddress("r.x", &r_x);
   t->SetBranchStatus("r.y", 1);
   t->SetBranchAddress("r.y", &r_y);
   t->SetBranchStatus("r.z", 1);
   t->SetBranchAddress("r.z", &r_z);




   // Output file
   std::ofstream _hepmc_file;

   //std::cout << outdir + nuwro_file_name + "_" + std::to_string(0) + ".hepmc" << std::endl;

   _hepmc_file.open(outdir + nuwro_file_name + "_" + std::to_string(0) + ".hepmc");


   double GlobalTimeOffset = 5627.5;
   double RandomTimeOffset = 9600.;

   srand(time(0));



   double entries = t->GetEntries();

   int i_collected=0; //number of events collected 

   for (size_t i = 0; i < entries; i++){

      if (i % 1000 == 0) std::cout << "At entry " << i << std::endl;

      if (i_collected % eventsperfile == 0) {

         // Close and open a new file

         _hepmc_file.close();
         _hepmc_file.open(outdir + nuwro_file_name + "_" + std::to_string(i_collected / eventsperfile) + ".hepmc");


      }

      t->GetEntry(i);

      bool lambda=false;

      //check if PV in fiducial volume
      //if( !inFV(TVector3(r_x,r_y,r_z)) )  continue;	


      //check if there is a Lambda in the final state
      for(int j=0;j<post_;j++){
         if(post_pdg[j] == 3122) lambda=true;
      }

      //skip event if no lambda in final state
      if(!lambda) continue;

      //check a nu was a muon antineutrino	
      if(in_pdg[0] != -14) continue;


      // Neutrino time in the spill
      double nu_time = rand() / double(RAND_MAX) * RandomTimeOffset + GlobalTimeOffset;

      // Save the number of particles for this events (+1 for the neutrino)
      _hepmc_file << i_collected << " " << post_ + in_ << std::endl;

      i_collected++;

      int ccnc = 0;
      if (!flag_cc) ccnc = 1;


      int mode = -1;
      if (flag_qel) mode = 0;
      if (flag_res) mode = 1;
      if (flag_dis) mode = 2;
      if (flag_coh) mode = 3;
      if (flag_mec) mode = 10;
      if (flag_hyp) mode = 1095; //correct MCNeutrino mode value for CCQEHyperon


      // Save the neutrino
      _hepmc_file << 0 << " "
         << in_pdg[0] << " "
         << -1 << " " << 0 << " " 
         << ccnc << " "                         // Should be 1st and 2nd daugther, but putting ccnc and mode
         << mode << " "                         // Should be 1st and 2nd daugther, but putting ccnc and mode
         << in_x[0] / 1000. << " "       // NuWro uses MeV, we want GeV
         << in_y[0] / 1000. << " "       // NuWro uses MeV, we want GeV
         << in_z[0] / 1000. << " "       // NuWro uses MeV, we want GeV
         << in_t[0] / 1000. << " "         // NuWro uses MeV, we want GeV
         << in__mass[0] / 1000. << " "
         << r_x << " "                       // NuWro uses cm, ok
         << r_y << " "                       // NuWro uses cm, ok
         << r_z << " "                       // NuWro uses cm, ok
         << nu_time << " "
         << std::endl;



      // Save other initial state particles
      for(size_t j = 1; j < in_; j++) {
         _hepmc_file << 11 << " "
            << in_pdg[j] << " "
            << -1 << " " << 0 << " " 
            << ccnc << " "                         // Should be 1st and 2nd daugther, but putting ccnc and mode
            << mode << " "                         // Should be 1st and 2nd daugther, but putting ccnc and mode
            << in_x[j] / 1000. << " "       // NuWro uses MeV, we want GeV
            << in_y[j] / 1000. << " "       // NuWro uses MeV, we want GeV
            << in_z[j] / 1000. << " "       // NuWro uses MeV, we want GeV
            << in_t[j] / 1000. << " "         // NuWro uses MeV, we want GeV
            << in__mass[j] / 1000. << " "
            << r_x << " "                       // NuWro uses cm, ok
            << r_y << " "                       // NuWro uses cm, ok
            << r_z << " "                       // NuWro uses cm, ok
            << nu_time << " "
            << std::endl;


      }



      // Save the final state particles
      for (size_t j = 0; j < post_; j++) {

         double mass;

         if(post_pdg[j] == 12 || post_pdg[j] == 14 || post_pdg[j] == 22)	mass = 0.0;
         else mass = post__mass[j]/1000.;		

         _hepmc_file << 1 << " "
            << post_pdg[j] << " "
            << 0 << " " << 0 << " " << 0 << " " << 0 << " "
            << post_x[j] / 1000. << " " // NuWro uses MeV, we want GeV
            << post_y[j] / 1000. << " " // NuWro uses MeV, we want GeV
            << post_z[j] / 1000. << " " // NuWro uses MeV, we want GeV
            << post_t[j] / 1000. << " "   // NuWro uses MeV, we want GeV
            << mass << " "
            << r_x << " "                   // NuWro uses cm, ok
            << r_y << " "                   // NuWro uses cm, ok
            << r_z << " "                   // NuWro uses cm, ok
            << nu_time << " "
            << std::endl;
      }


   }

   _hepmc_file.close();



   std::cout << i_collected << " events ready" << std::endl;


   std::ofstream output("eventcount.meta");
   output << i_collected;
   output.close();


}
