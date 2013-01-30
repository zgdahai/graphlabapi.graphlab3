#ifndef GRAPHLAB_DATABASE_GRAPH_EDGE_SHAREDMEM_HPP
#define GRAPHLAB_DATABASE_GRAPH_EDGE_SHAREDMEM_HPP
#include <graphlab/database/basic_types.hpp>
#include <graphlab/database/graph_row.hpp>
#include <graphlab/database/graph_edge.hpp>
namespace graphlab {

/**
 * \ingroup group_graph_database
 * An abstract interface for an edge of the graph.
 * The interface provides (locally cached) access to the data on the edge,
 * and provides control of synchronous and asynchronous modifications to the
 * edge data. The interface also provides access to adjacency information.
 *
 * This object is not thread-safe, and may not copied.
 *
 */
class graph_edge_sharedmem : public graph_edge {
 graph_vid_t sourceid;
 graph_vid_t targetid;
 graph_row* cache;
 graph_shard_id_t master;
 graph_database* database;
 public:
  graph_edge_sharedmem(const graph_vid_t& sourceid,
                       const graph_vid_t& targetid,
                       graph_row* data,
                       graph_shard_id_t master,
                       graph_database* database) :
  sourceid(sourceid), targetid(targetid), cache(data),
    master(master), database(database) {}

  /**
   * Returns the source ID of this edge
   */
  graph_vid_t get_src() { return sourceid; } 

  /**
   * Returns the destination ID of this edge
   */
  graph_vid_t get_dest() { return targetid; }

  /** Returns a pointer to the graph_row representing the data
   * stored on this edge. Modifications made to the data, are only committed 
   * to the database through a write_* call.
   *
   * \note Note that a pointer to the graph_row is returned. The graph_edge 
   * object retains ownership of the graph_row object. If this edge is freed 
   * (using \ref graph_database::free_edge ),  all pointers to the data 
   * returned by this function are invalidated.
   *
   * \note On the first call to data(), or all calls to *_refresh(), the 
   * graph_edge performs a synchronous read of the entire row from the
   * database, and caches it. Repeated calls to data() should always return
   * the same graph_row pointer.
   */
  graph_row* data()  {
    return cache;
  };

  // --- synchronization ---

  /**
   * Commits changes made to the data on this edge synchronously.
   * This resets the modification and delta flags on all values in the 
   * graph_row.
   *
   * \note Only values which have been modified should be sent 
   * (see \ref graph_value) and delta changes should be respected.
   * The function should also reset the modification flags, delta_commit flags
   * and update the _old values for each modified graph_value in the 
   * graph_row.
   */ 
  void write_changes() {
  };

  /**
   * Commits changes made to the data on this edge asynchronously.
   * This resets the modification and delta flags on all values in the 
   * graph_row.
   *
   * \note There are no guarantees as to when these modifications will be 
   * commited. Just that it will be committed eventually. The graph database
   * may buffer these modifications.
   *
   * \note Only values which have been modified should be sent 
   * (see \ref graph_value) and delta changes should be respected.
   * The function should also reset the modification flags, delta_commit flags
   * and update the _old values for each modified graph_value in the 
   * graph_row.
   */ 
  void write_changes_async()  {

  };

  /**
   * Synchronously refreshes the local copy of the data from the database, 
   * discarding all changes if any. This call may invalidate all previous
   * graph_row pointers returned by \ref data() . 
   *
   * \note The function should also reset the modification flags, delta_commit 
   * flags and update the _old values for each graph_value in the graph_row.
   */ 
  void refresh()  {
  };

  /**
   * Synchronously commits all changes made to the data on this edge, and
   * refreshes the local copy of the data from the database. Equivalent to a
   * a call to \ref write_changes() followed by \ref refresh() and may be 
   * implemented that way. This call may invalidate all previous
   * graph_row pointers returned by \ref data() . 
   */ 
  void write_and_refresh()  {
  };

  /**
   * Returns the ID of the shard owning this edge
   */
  graph_shard_id_t master_shard() {
    return master;
  };
};

} // namespace graphlab
#endif