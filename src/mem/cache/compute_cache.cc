#include "mem/cache/compute_cache.hh"
#include "base/logging.hh"
#include "base/trace.hh"
#include "base/types.hh"
#include "debug/ComputeCache.hh"
#include "params/ComputeCache.hh"
namespace gem5
{
ComputeCache::ComputeCache(const ComputeCacheParams &p)
: BaseCache(p, p.size / p.assoc),
    computeEnabled(p.compute_enabled),
    supportedOps(p.supported_ops.begin(), p.supported_ops.end()),
    computeLatency(p.compute_latency)
{
    DPRINTF(ComputeCache, "Created ComputeCache\n");
}

void 
ComputeCache::satisfyRequest(PacketPtr pkt, CacheBlk *blk, bool deferred_response, bool pending_downgrade) 
{
    if (computeEnabled && pkt->needCompute()) {
        if (performComputation(pkt)) {
            pkt->headerDelay += computeLatency;
            return;
        }
    }
    BaseCache::satisfyRequest(pkt, blk, deferred_response, pending_downgrade);
}

void
ComputeCache::vectorAdd(uint8_t* data, size_t size, int offset) 
{
    for (int i=0; i<size; i++) {
        data[i] += offset;
    }
}

bool
ComputeCache::performComputation(PacketPtr pkt) 
{
    if (supportedOps.find("ADD") != supportedOps.end()) {
        uint8_t* data = pkt->getPtr<uint8_t>();
        int32_t add_value = pkt->getComputeValue();
        size_t size = pkt->getSize();
        vectorAdd(data, size, add_value);
        DPRINTF(ComputeCache, "Performed ADD on %d bytes\n", size);
        return true;
    } 
    return false;
}

}