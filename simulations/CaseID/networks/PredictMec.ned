import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
import simu5g.common.binder.Binder;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.NR.NRUe;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.Upf;
import simu5g.nodes.mec.MECHost;
import simu5g.nodes.mec.MECOrchestrator.MecOrchestrator;
import simu5g.nodes.mec.UALCMP.UALCMP;
import simu5g.world.radio.LteChannelControl;
import ned.DatarateChannel;
import inet.node.internetcloud.InternetCloud;
import inet.networklayer.internetcloud.MatrixCloudDelayer;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.transportlayer.udp;


network PredictMec
{
    parameters:
        @display("i=block/network2,,0;bgb=1016,701,,,0;bgi=background/streetmap");

        int numUes = default(8);
        int numMalUes = default(5);
        int numVidUes = default(2);
        int numBgCells = default(0);

    types:
        channel C extends DatarateChannel
        {
            datarate = 100Mbps;
            delay = 0.1us;
        }

    submodules:



        routingRecorder: RoutingTableRecorder {
            @display("p=641,453;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=734,453;is=s");
        }
        channelControl: LteChannelControl {
            @display("p=786,399;is=s");
        }
        binder: Binder {
            @display("p=820,453;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=927,446;is=s");
        }

        internetCloud: InternetCloud {
            @display("p=717,180");
        }
        localRouter: Router {
            @display("p=532,60");
        }
        remoteServer: StandardHost {
            @display("p=851,168;is=n;i=device/server");
        }
        videoServer: StandardHost {
            @display("p=881,59;is=n;i=device/server");
        }

        upf: Upf {
            @display("p=551,211");
        }
        iUpf: Upf {
            @display("p=373,316");
        }
        gNodeB1: gNodeB {
            @display("p=819,330;is=vl");
        }
        malUe[numMalUes]: NRUe {
            @display("p=439,399");
        }
        malVidUe[numMalUes]: NRUe {
            @display("p=187,446");
        }
        Ue[numUes]: NRUe {
            @display("p=470,608");
        }

        VidUe[numVidUes]: NRUe {
            @display("p=717,594");
        }
        //# MEC module
        mecHost: MECHost {
            @display("p=389,86;is=l");
        }

        mecOrchestrator: MecOrchestrator {
            @display("p=199,104;is=l");
        }

        ualcmp: UALCMP {
            @display("p=121,247;i=device/smallrouter");
        }

//        messageSender: MessageSender {   
//        	@display("p=100,100;i=device/smallrouter");
//        }


    connections:

        // Local Router to Remote Server
        remoteServer.pppg++ <--> Eth10G <--> localRouter.pppg++;

        // Video server to localRouter. By only having one router we only  need to capture one PCAP
        videoServer.pppg++ <--> Eth10G <--> localRouter.pppg++;

        // Local Router to InternetCloud
        localRouter.pppg++ <--> Eth10G <--> internetCloud.pppg++;

        //UPF to internetCloud
        internetCloud.pppg++ <--> Eth10G { @display("ls=,0"); } <--> upf.filterGate;

        upf.pppg++ <--> Eth10G <--> iUpf.pppg++;
        ualcmp.ppp++ <--> Eth10G <--> upf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        ualcmp.toMecOrchestrator --> mecOrchestrator.fromUALCMP;
        ualcmp.fromMecOrchestrator <-- mecOrchestrator.toUALCMP;

        mecHost.ppp++ <--> Eth10G <--> iUpf.pppg++;
}
