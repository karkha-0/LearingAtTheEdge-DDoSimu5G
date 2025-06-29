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

//import ddosim5g.trafficcontroller.DataTrafficController;
import inet.transportlayer.udp;

// This is a simple network, used to test generating benign data.
// The goal is to have one or two UEs, one gnb and one remote server

network Benign
{
    parameters:
        int numUe = default(1);
        int numMalUe = default(1);
        int numVidUe = default(1);
        int numBgCells = default(0);
        @display("i=block/network2;bgb=3147,1999;bgi=background/lthmodified");

    types:
        channel C extends DatarateChannel
        {
            datarate = 100Mbps;
            delay = 0.1us;
        }

    submodules:

        channelControl: LteChannelControl {
            @display("p=50,25;is=s");
        }
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
        }
        binder: Binder {
            @display("p=50,175;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=50.993748,258.7;is=s");
        }
        internetCloud: InternetCloud {
            @display("p=1010.016,172.344");
        }
        localRouter: Router {
            @display("p=701.39996,172.344");
        }
        remoteServer: StandardHost {
            @display("p=416.832,172.344;is=n;i=device/server");
        }
        videoServer: StandardHost {
            @display("p=236.472,741.48;is=n;i=device/server");
        }
        upf: Upf {
            @display("p=1430.856,172.344");
        }
        iUpf: Upf {
            @display("p=1823.6399,172.344");
        }
        gnb1: gNodeB {
            @display("p=1500,750;is=l");

        }
        malUe[numMalUe]: NRUe {
            @display("p=571,940");
        }
        malVidUe[numMalUe]: NRUe {
            @display("p=1182,800");
        }
        ue[numUe]: NRUe {
            @display("p=900,1080");
        }
        VidUe[numVidUe]: NRUe {
            @display("p=1365,1082");
        }
//        videoRouter: Router {
//            @display("p=236.472,432.86398");
//        }

    connections:
        //# Data network
        // Local Router to Remote Server
        remoteServer.pppg++ <--> Eth10G <--> localRouter.pppg++;

        // Video server to localRouter. By only having one router we only  need to capture one PCAP
        videoServer.pppg++ <--> Eth10G <--> localRouter.pppg++;

        // Local Router to InternetCloud
        localRouter.pppg++ <--> Eth10G <--> internetCloud.pppg++;

        // Video Router to Video Server
       // videoServer.pppg++ <--> Eth10G <--> videoRouter.pppg++;
        // Video Router to InternetCloud
       // videoRouter.pppg++ <--> Eth10G <--> internetCloud.pppg++;

        //UPF to internetCloud
        internetCloud.pppg++ <--> Eth10G <--> upf.filterGate;

        //# 5G Core Network connections
        // UPF to iUPF
        upf.pppg++ <--> Eth10G <--> iUpf.pppg++;
        //iUpf.ethg++ <--> Eth10G <--> upf.ethg++;
        //iUpf.pppg++ <--> Eth10G <--> upf.pppg++;
        //iUPF tp gnb
        iUpf.pppg++ <--> Eth10G <--> gnb1.ppp;
}
