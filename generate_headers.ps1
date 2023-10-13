#---------ZOSAPI & CataHash header generator for zos.h---------------
# Build process:
# 1. Run this script to generate TLB's and catahash.h
# 2. Build in release config so that MSVC creates TLH files from TLB
# 3. Switch to debug: now MSVS is using TLH headers which makes it easier for Intellisense

param(
        [Parameter(
            Mandatory=$True,
            ValueFromRemainingArguments=$true,
            Position = 0
        )][string[]]
        $glasses
)

$zmxdir = $(Join-Path $([environment]::getfolderpath("mydocuments")) "Zemax")
$outdir = "build\generated"

cp $(Join-Path $zmxdir "ZOS-API\Libraries\*.tlb") $outdir

# -----------Glass catalog hash-------------------------
rm $(Join-Path $outdir glasscat.agf)
foreach($glass in $glasses){
    Get-content $(Join-Path $zmxdir $("Glasscat\"+$glass+".agf")) -Raw `
        | out-file $(Join-Path $outdir glasscat.agf) -Encoding ascii -append
}

# note: in a PS pipe, arrays of strings are passed, not bin streams, so we manually concatenate them with LF...
($(awk -f zosutils\agf2gperf.awk $(Join-Path $outdir glasscat.agf)) -join "`n") + "`n" `
    | out-file $(Join-Path $outdir keyfile.gperf) -Encoding ascii -NoNewline # ...and do not let PS append a CRLF
# - since MinGW builds of gperf can't read CRLF
gperf --struct-type --readonly-tables --includes --hash-function-name=catahash --lookup-function-name=get_pGlass `
--word-array-name=glasses --multiple-iterations=10 `
$(Join-Path $outdir keyfile.gperf) | out-file $(Join-Path $outdir catahash.h) -Encoding ascii

exit 0