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

        bool computeEnabled;
        std::unordered_set<std::string> supportedOps;
        Cycles computeLatency;

        void vectorAdd(uint8_t* data, size_t size, int offset = 1);

        bool performComputation(PacketPtr pkt);

    public:
        ComputeCache(const ComputeCacheParams &p);

        virtual void satisfyRequest(PacketPtr pkt, CacheBlk *blk,
            bool deferred_response, bool pending_downgrade) override;
};
}

#endif //__MEM_CACHE_COMPUTE_CACHE_HH__