#include "mem/cache/compute_cache.hh"
#include "base/logging.hh"
#include "base/trace.hh"
#include "base/types.hh"
#include "debug/ComputeCache.hh"
#include "params/ComputeCache.hh"
namespace gem5
{
ComputeCache::ComputeCache(const BaseCacheParams *params)
: BaseCache(params)
{
    computeFuncs[IntAdd] = [](uint8_t* data, int operand) {
        *reinterpret_cast<int*>(data) += operand;
    };
    
    computeFuncs[IntMul] = [](uint8_t* data, int operand) {
        *reinterpret_cast<int*>(data) *= operand;
    };
}

void
ComputeCache::recvTimingReq(PacketPtr pkt) {
    if (pkt->cmd == MemCmd::ComputeReq) {
        handleComputeRequest(pkt);
    } else {
        BaseCache::recvTimingReq(pkt);
    }
}
void 
ComputeCache::satisfyRequest(PacketPtr pkt, CacheBlk *blk, bool deferred_response, bool pending_downgrade) 
{
    if (pkt->cmd == MemCmd::ComputeReq) {
        if (tags->findBlock(pkt->getAddr())->isValid()) {
            processCompute(pkt);
        }
    } else {
        Cache::satisfyRequest(pkt, blk, deferred_response, pending_downgrade);
    }
}

void 
ComputeCache::handleComputeRequest(PacketPtr pkt) {
    if (!tags->findBlock(pkt->getAddr())->isValid()) {
        PacketPtr fillPkt = new Packet(pkt->req, MemCmd::ReadReq);
        memSidePort.sendTimingReq(fillPkt); 
        pkt->pushSenderState(new ComputeCallback(this));
    }
}

void
ComputeCache::processCompute(PacketPtr pkt) {
    auto compute_type = static_cast<ComputeType>(pkt->getComputeType());
    int operand = pkt->getComputeOperand();
    
    uint8_t* data = pkt->getPtr();
    computeFuncs[compute_type](data, operand);
    
    tags->findBlock(pkt->getAddr())->setDirty();
    pkt->makeResponse();
    cpuSidePort.sendTimingResp(pkt);
}

}