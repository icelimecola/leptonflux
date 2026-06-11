# default
tbin01->GetEntries()
tbin01->Show(0)
tbin01->GetEntries("isbadrun")
tbin01->GetEntries("!isbadrun")
# gcx
tbin01->GetEntries("isbadrun_daq || isbadrun_fov || isbadrun_ecal || isbadrun_trk")
tbin01->GetEntries("utime>=1305417600 && utime<=1307750400")
tbin01->GetEntries("utime>=1305417600 && utime<=1307750400 && isbadrun")
tbin01->GetEntries("utime>=1305417600 && utime<=1307750400 && !isbadrun")
tbin01->GetEntries("utime>=1305417600 && utime<=1307750400 && isbadrun_general")
# ====2
tbin01->Scan("run:event","utime>=1305417600 && utime<=1307750400 && !isbadrun")

# ======== tsu ========
# ====cd
cd /eos/ams/user/t/tsu/ECAL_buffer_pass8
# ====1
tb->GetEntries()
tb->GetEntries("!isbadrun")
