#ifdef NS3_MODULE_COMPILATION 
    error "Do not include ns3 module aggregator headers from other modules these are meant only for end user scripts." 
#endif 
#ifndef NS3_MODULE_SIONNA
    // Module headers: 
    #include <ns3/sionna-connection-handler.h>
    #include <ns3/sionna-helper.h>
#endif 