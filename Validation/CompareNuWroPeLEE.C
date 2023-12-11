
enum mode {kQEL,kRES,kDIS,kCOH,kMEC,kHYP,kMAX};
const std::vector<std::string> mode_str = {"QEL","RES","DIS","COH","MEC","HYP"};

// Compare a NuWro ntuple to a pelee ntuple for validation purposes

void CompareNuWroPeLEE(){

  std::string nuwro_file = "uboone_BNB_run4b_2023.root";
  const double nuwro_POT = 2.99899e+20; 

  std::string pelee_file = "high_stat_prodgenie_bnb_nu_overlay_DetVar_Run1_NuWro_reco2_reco2.root";
  const double pelee_POT = 3.08e20;

  TFile* f_nuwro = TFile::Open(nuwro_file.c_str()); 
  TFile* f_pelee = TFile::Open(pelee_file.c_str()); 

  TTree* t_nuwro = static_cast<TTree*>(f_nuwro->Get("treeout"));
  TTree* t_pelee = static_cast<TTree*>(f_pelee->Get("nuselection/NeutrinoSelectionFilter"));

  std::vector<TH1D*> h_NuE_v_nuwro;
  std::vector<TH1D*> h_nu_vtx_x_v_nuwro;
  std::vector<TH1D*> h_nu_vtx_y_v_nuwro;
  std::vector<TH1D*> h_nu_vtx_z_v_nuwro;

  std::vector<TH1D*> h_NuE_v_pelee;
  std::vector<TH1D*> h_nu_vtx_x_v_pelee;
  std::vector<TH1D*> h_nu_vtx_y_v_pelee;
  std::vector<TH1D*> h_nu_vtx_z_v_pelee;

  for(int i=0;i<kMAX;i++){
    h_NuE_v_nuwro.push_back(new TH1D(("h_NuE_nuwro_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu E (GeV);Events/10^{20} POT").c_str(),20,0.0,2.0));
    h_nu_vtx_x_v_nuwro.push_back(new TH1D(("h_nu_vtx_x_nuwro_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu Vertex X (cm);Events/10^{20} POT").c_str(),20,-100,350));
    h_nu_vtx_y_v_nuwro.push_back(new TH1D(("h_nu_vtx_y_nuwro_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu Vertex Y (cm);Events/10^{20} POT").c_str(),20,-250,250));
    h_nu_vtx_z_v_nuwro.push_back(new TH1D(("h_nu_vtx_z_nuwro_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu Vertex Z (cm);Events/10^{20} POT").c_str(),20,-200,1200));

    h_NuE_v_pelee.push_back(new TH1D(("h_NuE_pelee_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu E (GeV);Events/10^{20} POT").c_str(),20,0.0,2.0));
    h_nu_vtx_x_v_pelee.push_back(new TH1D(("h_nu_vtx_x_pelee_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu Vertex X (cm);Events/10^{20} POT").c_str(),20,-100,350));
    h_nu_vtx_y_v_pelee.push_back(new TH1D(("h_nu_vtx_y_pelee_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu Vertex Y (cm);Events/10^{20} POT").c_str(),20,-250,250));
    h_nu_vtx_z_v_pelee.push_back(new TH1D(("h_nu_vtx_z_pelee_"+mode_str.at(i)).c_str(),(mode_str.at(i)+";"+"#nu Vertex Z (cm);Events/10^{20} POT").c_str(),20,-200,1200));
  }

  // pelee branches
  Int_t interaction;
  Int_t nu_pdg;
  Float_t nu_e;
  Float_t true_nu_vtx_x;
  Float_t true_nu_vtx_y;
  Float_t true_nu_vtx_z;
  t_pelee->SetBranchAddress("interaction",&interaction);
  t_pelee->SetBranchAddress("nu_pdg",&nu_pdg);
  t_pelee->SetBranchAddress("nu_e",&nu_e);
  t_pelee->SetBranchAddress("true_nu_vtx_x",&true_nu_vtx_x);
  t_pelee->SetBranchAddress("true_nu_vtx_y",&true_nu_vtx_y);
  t_pelee->SetBranchAddress("true_nu_vtx_z",&true_nu_vtx_z);

  // Fill the pelee histograms
  for(Long64_t ientry=0;ientry<t_pelee->GetEntries();ientry++){
    t_pelee->GetEntry(ientry);

    int mode = -1; 
    if(interaction == 0) mode = kQEL;
    if(interaction == 1) mode = kRES;
    if(interaction == 2) mode = kDIS;
    if(interaction == 3) mode = kCOH;
    if(interaction == 10) mode = kMEC;
    if(interaction == 1095) mode = kHYP;

    if(mode != -1){
      h_NuE_v_pelee.at(mode)->Fill(nu_e);     
      h_nu_vtx_x_v_pelee.at(mode)->Fill(true_nu_vtx_x);     
      h_nu_vtx_y_v_pelee.at(mode)->Fill(true_nu_vtx_y);     
      h_nu_vtx_z_v_pelee.at(mode)->Fill(true_nu_vtx_z);     
    }

  }

  // nuwro branches
  static constexpr Int_t kMaxin = 3;
  Bool_t          flag_qel;
  Bool_t          flag_res;
  Bool_t          flag_dis;
  Bool_t          flag_coh;
  Bool_t          flag_mec;
  Bool_t          flag_hyp;
  Bool_t          flag_nc;
  Bool_t          flag_cc;
  Int_t           in_;
  Double_t        in_t[kMaxin];   //[in_]
  Double_t        r_x;
  Double_t        r_y;
  Double_t        r_z;

  t_nuwro->SetMakeClass(1); // tree in MakeClass mode
  t_nuwro->SetBranchStatus("*", 0); // disable all branches

  t_nuwro->SetBranchStatus("flag.qel",1);
  t_nuwro->SetBranchStatus("flag.res",1);
  t_nuwro->SetBranchStatus("flag.dis",1);
  t_nuwro->SetBranchStatus("flag.coh",1);
  t_nuwro->SetBranchStatus("flag.hyp",1);
  t_nuwro->SetBranchStatus("flag.mec",1);
  t_nuwro->SetBranchStatus("in",1);
  t_nuwro->SetBranchStatus("in.t",1);
  t_nuwro->SetBranchStatus("r",1);
  t_nuwro->SetBranchStatus("r.x",1);
  t_nuwro->SetBranchStatus("r.y",1);
  t_nuwro->SetBranchStatus("r.z",1);
  t_nuwro->SetBranchAddress("flag.qel", &flag_qel);
  t_nuwro->SetBranchAddress("flag.res", &flag_res);
  t_nuwro->SetBranchAddress("flag.dis", &flag_dis);
  t_nuwro->SetBranchAddress("flag.coh", &flag_coh);
  t_nuwro->SetBranchAddress("flag.hyp", &flag_hyp);
  t_nuwro->SetBranchAddress("flag.mec", &flag_mec);
  t_nuwro->SetBranchAddress("in",&in_);
  t_nuwro->SetBranchAddress("in.t",in_t);
  t_nuwro->SetBranchAddress("r.x", &r_x);
  t_nuwro->SetBranchAddress("r.y", &r_y);
  t_nuwro->SetBranchAddress("r.z", &r_z);

  // fill the nuwro trees
  for(Long64_t ientry=0;ientry<t_nuwro->GetEntries();ientry++){
    t_nuwro->GetEntry(ientry);

    int mode = -1;
    if(flag_qel) mode = kQEL;
    if(flag_res) mode = kRES;
    if(flag_dis) mode = kDIS;
    if(flag_coh) mode = kCOH;
    if(flag_mec) mode = kMEC;
    if(flag_hyp) mode = kHYP;

    if(mode != -1){
      h_NuE_v_nuwro.at(mode)->Fill(in_t[0]/1e3);     
      h_nu_vtx_x_v_nuwro.at(mode)->Fill(r_x);     
      h_nu_vtx_y_v_nuwro.at(mode)->Fill(r_y);     
      h_nu_vtx_z_v_nuwro.at(mode)->Fill(r_z);     
    }

  }

  // Draw the plots!
  TCanvas* c = new TCanvas("c","c"); 
  gSystem->Exec("mkdir -p Plots/");

  // Scale everything to POT
  for(int i=0;i<kMAX;i++){
    h_NuE_v_nuwro.at(i)->Scale(1e20/nuwro_POT);
    h_NuE_v_pelee.at(i)->Scale(1e20/pelee_POT);

    h_NuE_v_nuwro.at(i)->SetLineWidth(2);
    h_NuE_v_nuwro.at(i)->SetLineColor(2);
    h_NuE_v_nuwro.at(i)->Draw("HIST");

    h_NuE_v_pelee.at(i)->SetLineWidth(2);
    h_NuE_v_pelee.at(i)->SetLineColor(3);
    h_NuE_v_pelee.at(i)->Draw("HIST same");

    c->Print(("Plots/NuE_"+mode_str.at(i)+".png").c_str());
    c->Clear();

    h_nu_vtx_x_v_nuwro.at(i)->Scale(1e20/nuwro_POT);
    h_nu_vtx_x_v_pelee.at(i)->Scale(1e20/pelee_POT);

    h_nu_vtx_x_v_nuwro.at(i)->SetLineWidth(2);
    h_nu_vtx_x_v_nuwro.at(i)->SetLineColor(2);
    h_nu_vtx_x_v_nuwro.at(i)->Draw("HIST");

    h_nu_vtx_x_v_pelee.at(i)->SetLineWidth(2);
    h_nu_vtx_x_v_pelee.at(i)->SetLineColor(3);
    h_nu_vtx_x_v_pelee.at(i)->Draw("HIST same");

    c->Print(("Plots/nu_vtx_x_"+mode_str.at(i)+".png").c_str());
    c->Clear();

    h_nu_vtx_y_v_nuwro.at(i)->Scale(1e20/nuwro_POT);
    h_nu_vtx_y_v_pelee.at(i)->Scale(1e20/pelee_POT);

    h_nu_vtx_y_v_nuwro.at(i)->SetLineWidth(2);
    h_nu_vtx_y_v_nuwro.at(i)->SetLineColor(2);
    h_nu_vtx_y_v_nuwro.at(i)->Draw("HIST");

    h_nu_vtx_y_v_pelee.at(i)->SetLineWidth(2);
    h_nu_vtx_y_v_pelee.at(i)->SetLineColor(3);
    h_nu_vtx_y_v_pelee.at(i)->Draw("HIST same");

    c->Print(("Plots/nu_vtx_y_"+mode_str.at(i)+".png").c_str());
    c->Clear();

    h_nu_vtx_z_v_nuwro.at(i)->Scale(1e20/nuwro_POT);
    h_nu_vtx_z_v_pelee.at(i)->Scale(1e20/pelee_POT);

    h_nu_vtx_z_v_nuwro.at(i)->SetLineWidth(2);
    h_nu_vtx_z_v_nuwro.at(i)->SetLineColor(2);
    h_nu_vtx_z_v_nuwro.at(i)->Draw("HIST");

    h_nu_vtx_z_v_pelee.at(i)->SetLineWidth(2);
    h_nu_vtx_z_v_pelee.at(i)->SetLineColor(3);
    h_nu_vtx_z_v_pelee.at(i)->Draw("HIST same");

    c->Print(("Plots/nu_vtx_z_"+mode_str.at(i)+".png").c_str());
    c->Clear();



  }

  c->Close();

}
