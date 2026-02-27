#ifndef LHC_h
#define LHC_h

struct SimpleTime {
  uint8_t hours;
  uint8_t mins;
};

struct BisSmpFlags {
  bool permits_linked;
  bool beam_permit;
  bool setup_beam;
  bool beam_present;
  bool movables_allowed_in;
  bool stable_beams;
};

struct LhcExperiment {
  // TODO: more stuff in the experiments
  double  inst_lumi;
  
  uint8_t changed_fields; // bit field composed of LHC_EXP_CHANGE_* values
};

class LargeHadronCollider {
  public:
    LargeHadronCollider();
    void refresh();

    char                data_url[60];
    uint16_t            energy;
    char                comment[255];
    char                accel_mode[17];
    char                beam_mode[17];
    struct SimpleTime   t_sb;
    struct BisSmpFlags  flags[2];
    uint16_t            total_bunches;
    bool                cryo_ok;

    struct LhcExperiment  experiment[3];

    uint8_t             changed_fields; // bit field composed of LHC_CHANGE_* values
};

#define CERNDATA_PROD_URL     "http://cerndisplay.kitrich.net/vistars.json"
#define CERNDATA_TEST_URL     "http://cerndisplay.kitrich.net/vistars-test.json"

#define LHC_CHANGE_ENERGY     0x01
#define LHC_CHANGE_COMMENT    0x02
#define LHC_CHANGE_ACCEL_MODE 0x04
#define LHC_CHANGE_BEAM_MODE  0x08
#define LHC_CHANGE_T_SB       0x10
#define LHC_CHANGE_FLAGS      0x20
#define LHC_CHANGE_BUNCHES    0x40
#define LHC_CHANGE_CRYO       0x80

#define LHC_EXP_ATLAS         0
#define LHC_EXP_ALICE         1
#define LHC_EXP_CMS           2
#define LHC_EXP_LHCB          3

#define LHC_EXP_CHANGE_INSTLUMI 1   // instantaneous lumi changed from zero to non-zero or vice-versa

#endif