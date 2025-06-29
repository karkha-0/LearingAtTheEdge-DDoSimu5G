
import inet.networklayer.internetcloud.MatrixCloudDelayer;
import inet.node.internetcloud.InternetCloud;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.NR.NRUe;
import simu5g.common.binder.Binder;
import simu5g.world.radio.LteChannelControl;
import simu5g.common.carrierAggregation.CarrierAggregation;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import simu5g.nodes.Upf;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
import ned.DatarateChannel;
import ddosim5g.simcommands.PostSimulationCommand;


network BasicNetwork
{
    parameters:
        int numUe = default(2);
        @display("i=block/network2;bgb=2000,2000;bgi=background/lthmodified");
        
    types:
        channel C extends DatarateChannel
        {
            datarate = 100Mbps;
            delay = 0.1us;
        }
        
     submodules:
         
         gnb: gNodeB {
            @display("p=726.35,277.35626;is=vl");
        }
        
        ue[numUe]: NRUe {
            @display("p=628.09375,411.68124");
        }
        
        
        
        
     connections:
         
     	
   
}
