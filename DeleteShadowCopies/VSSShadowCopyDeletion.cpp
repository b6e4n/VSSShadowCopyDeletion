
//Highly took from the Microsoft implementation https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/winbase/vss/vshadow/delete.cpp but directly usable :)


#include <Windows.h>
#include <stdio.h>
#include <vss.h>
#include <vswriter.h>
#include <vsbackup.h>
#include <atlcomcli.h>


int main() {
    
    HRESULT hr;
    IVssBackupComponents* m_pVssObject;

    //Initialization following https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/winbase/vss/vshadow/vssclient.cpp
    hr = CoInitialize(NULL);
    hr = CoInitializeSecurity(
        NULL,                           //  Allow *all* VSS writers to communicate back!
        -1,                             //  Default COM authentication service
        NULL,                           //  Default COM authorization service
        NULL,                           //  reserved parameter
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,  //  Strongest COM authentication level
        RPC_C_IMP_LEVEL_IDENTIFY,       //  Minimal impersonation abilities 
        NULL,                           //  Default COM authentication settings
        EOAC_NONE,                      //  No special options
        NULL                            //  Reserved parameter
    );

    
    //Following the vssclient.cpp initialization, Doc for IVssBackupComponents m_pVssObject: https://learn.microsoft.com/en-us/windows/win32/api/vsbackup/nl-vsbackup-ivssbackupcomponents
    hr = CreateVssBackupComponents(&m_pVssObject);
    if (hr == E_ACCESSDENIED) {
        printf("Need Administrator privileges\n");
        return 0;
    }
    m_pVssObject->InitializeForBackup();
    m_pVssObject->SetContext(VSS_CTX_ALL); //dwContext = VSS_CTX_ALL --> All types of currently live shadow copies are available for administrative operations
    m_pVssObject->SetBackupState(true, true, VSS_BT_FULL, false);


    // Get list all shadow copies. 
    CComPtr<IVssEnumObject> pIEnumSnapshots;
    hr = m_pVssObject->Query(GUID_NULL,VSS_OBJECT_NONE,VSS_OBJECT_SNAPSHOT,&pIEnumSnapshots);
    if (hr == S_FALSE)
    {
        printf("\nThere are no shadow copies on the system\n");
        return 0;
    }

    VSS_OBJECT_PROP Prop;
    VSS_SNAPSHOT_PROP& Snap = Prop.Obj.Snap;

    while (true) {
        ULONG ulFetched;
        hr = pIEnumSnapshots->Next(1, &Prop, &ulFetched);

        if (ulFetched == 0) {
            printf("Reached the end of list\n");
            break;
        }

        VssFreeSnapshotProperties(&Snap);

        // Perform the actual deletion
        LONG lSnapshots = 0;
        VSS_ID idNonDeletedSnapshotID = GUID_NULL;
        hr = m_pVssObject->DeleteSnapshots(
            Snap.m_SnapshotId,
            VSS_OBJECT_SNAPSHOT,
            FALSE,
            &lSnapshots,
            &idNonDeletedSnapshotID);
    }
   
    return 1;
}

