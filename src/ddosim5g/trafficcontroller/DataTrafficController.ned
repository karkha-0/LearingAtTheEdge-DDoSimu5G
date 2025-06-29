package ddosim5g.trafficcontroller;

simple DataTrafficController {
    parameters:
        @class(DataTrafficController);  // Ensures the NED file references the correct C++ class
        string infectionFilePath;
        
        @signal[infectionEventTime]; 
        @statistic[infectionEventTime](title="Imported infection data "; unit=""; source="infectionEventTime"; record=vector);
}