#ifndef __MEM_CACHE_COMPUTE_CACHE_HH__
#define __MEM_CACHE_COMPUTE_CACHE_HH__

#include "mem/cache/base.hh"
#include "params/ComputeCache.hh"
#include "mem/packet.hh"
#include "base/compiler.hh"
#include "base/logging.hh"
#include "base/types.hh"

namespace gem5
{
struct ComputeCacheParams;
class ComputeCache : public BaseCache
{
    protected:
        enum ComputeType { 
            IntAdd,   
            IntMul,
        };
        Cycles computeLatency;

        bool processCompute(PacketPtr pkt);
        void handleComputeRequest(PacketPtr pkt);
        std::unordered_map<ComputeType, std::function<void(uint8_t*, int)>> computeFuncs;
        void recvTimingReq(PacketPtr pkt) override;

    public:
        ComputeCache(const BaseCacheParams *params);

        virtual void satisfyRequest(PacketPtr pkt, CacheBlk *blk,
            bool deferred_response, bool pending_downgrade) override;
};
}

#endif //__MEM_CACHE_COMPUTE_CACHE_HH__