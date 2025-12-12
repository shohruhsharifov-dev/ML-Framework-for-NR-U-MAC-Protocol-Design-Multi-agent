#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_NR_U
    // Module headers: 
    #include <ns3/nr-on-off-access-manager.h>
    #include <ns3/nr-lbt-access-manager.h>
    #include <ns3/application-scenario.h>
    #include <ns3/interference-application.h>
    #include <ns3/in-front-node-distribution.h>
    #include <ns3/l2-setup.h>
    #include <ns3/no-interference-node-distribution.h>
    #include <ns3/node-distribution-scenario.h>
    #include <ns3/nr-single-bwp-setup.h>
    #include <ns3/single-pair-node-scenario.h>
    #include <ns3/varying-interference-node-distribution.h>
    #include <ns3/wigig-setup.h>
    #include <ns3/output-manager.h>
    #include <ns3/file-output-manager.h>
    #include <ns3/half-matrix-layout.h>
    #include <ns3/wifi-setup.h>
    #include <ns3/nr-u-trace-helper.h>
#endif 