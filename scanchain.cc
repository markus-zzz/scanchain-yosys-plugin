#include "kernel/yosys.h"
USING_YOSYS_NAMESPACE

class ScanChain : public Pass {

  void stitch(RTLIL::Design *design, RTLIL::Module *module) {
    (void)design;
    log("Starting scan chain stitching for module '%s'.\n", log_id(module));

    scan_en = module->addWire("\\scan_en", 1);
    scan_en->port_input = true;
    scan_en->port_id = 0;

    scan_in = module->addWire("\\scan_in", 1);
    scan_in->port_input = true;
    scan_in->port_id = 0;

    scan_out = module->addWire("\\scan_out", 1);
    scan_out->port_output = true;
    scan_out->port_id = 0;

    scan_len = module->addWire("\\scan_len", 32);
    scan_len->port_output = true;
    scan_len->port_id = 0;

    module->fixup_ports();

    RTLIL::SigSpec scan_tmp = RTLIL::SigSpec(scan_in);
    uint32_t stitched_flops = 0;

    for (auto &cit : module->cells_) {
      auto *cell = cit.second;
      if (cell->type != ID($dff))
        continue;

      RTLIL::Wire *y = module->addWire(NEW_ID, 1);
      module->addMux(NEW_ID, cell->getPort(ID::D), scan_tmp, scan_en, y);
      cell->setPort(ID::D, y);

      scan_tmp = cell->getPort(ID::Q);
      stitched_flops++;
    }
    module->connect(scan_out, scan_tmp);
    module->connect(scan_len, RTLIL::Const(stitched_flops, 32));
    log("Stitched %u flops.\n", stitched_flops);
  }

  RTLIL::Wire *scan_en;
  RTLIL::Wire *scan_in;
  RTLIL::Wire *scan_out;
  RTLIL::Wire *scan_len;

public:
  ScanChain() : Pass("scanchain", "Scan chain stitch all FFs in module") {}
  void execute(std::vector<std::string>, RTLIL::Design *design) override {
    log_header(design,
               "Executing SCANCHAIN pass (scan chain stitch all FFs).\n");
    for (auto &it : design->modules_)
      if (design->selected_module(it.first))
        stitch(design, it.second);
  }
} ScanChain;
