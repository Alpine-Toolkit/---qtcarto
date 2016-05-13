// -*- mode: c++ -*-

/***************************************************************************************************
**
** $QTCARTO_BEGIN_LICENSE:GPL3$
**
** Copyright (C) 2016 Fabrice Salvaire
** Contact: http://www.fabrice-salvaire.fr
**
** This file is part of the QtCarto library.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QTCARTO_END_LICENSE$
**
***************************************************************************************************/

/**************************************************************************************************/

#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

/**************************************************************************************************/

#include <QObject>
#include <QSize>

#include "qtcarto_global.h"
#include "coordinate/geo_coordinate.h"
#include "geometry/polygon.h"
#include "math/interval.h"

/**************************************************************************************************/

// QC_BEGIN_NAMESPACE

/**************************************************************************************************/

/*!
 *
 * Zoom factor unit is [m/px]
 *
 */
// Fixme: QcZoomFactor -> QcMapResolution ?
class QC_EXPORT QcZoomFactor
{
 public:
  QcZoomFactor();
  QcZoomFactor(double zoom_factor);
  QcZoomFactor(const QcZoomFactor & other);
  ~QcZoomFactor();

  QcZoomFactor & operator=(const QcZoomFactor & other);

  inline bool operator==(const QcZoomFactor & other) const {
    return m_zoom_factor == other.m_zoom_factor;
  }

  inline bool operator!=(const QcZoomFactor & other) const {
    return !operator==(other);
  }

  double zoom_factor() const { return m_zoom_factor; }
  void set_zoom_factor(double zoom_factor);

 private:
  double m_zoom_factor;
};

/**************************************************************************************************/

// Tool to compute a zoom factor from a pyramid
// Fixme: versus tile matrix ???
class QC_EXPORT QcTiledZoomLevel : public QcZoomFactor
{
 public:
  QcTiledZoomLevel();
  QcTiledZoomLevel(double map_size, unsigned int tile_size, unsigned int zoom_level = 0); // Fixme: tile_size depends of the map 256, 512 !!!
  QcTiledZoomLevel(const QcTiledZoomLevel & other);
  ~QcTiledZoomLevel();

  QcTiledZoomLevel & operator=(const QcTiledZoomLevel & other);

  bool operator==(const QcTiledZoomLevel & other) const;

  inline bool operator!=(const QcTiledZoomLevel & other) const {
    return !operator==(other);
  }

  double map_size() const { return m_map_size; }

  unsigned int tile_size() const { return m_tile_size; }

  unsigned int zoom_level() const { return m_zoom_level; }
  void set_zoom_level(unsigned int zoom_level);

 private:
  unsigned int m_tile_size;
  unsigned int m_zoom_level;
  double m_map_size;
};

/**************************************************************************************************/

class QC_EXPORT QcViewportState
{
 public:
  inline bool is_valid_bearing(double bearing) {
    return -180. <= bearing && bearing <= 180.;
  }

 public:
  QcViewportState();
  // Fixme: QcTiledZoomLevel or QcZoomFactor ???
  QcViewportState(const QcGeoCoordinateWGS84 & coordinate, const QcTiledZoomLevel & tiled_zoom_level, double bearing);
  QcViewportState(const QcGeoCoordinateWebMercator & coordinate, const QcTiledZoomLevel & tiled_zoom_level, double bearing);
  QcViewportState(const QcGeoCoordinatePseudoWebMercator & coordinate, const QcTiledZoomLevel & tiled_zoom_level, double bearing);
  // QcViewportState(const QcGeoCoordinateNormalisedWebMercator & coordinate, const QcTiledZoomLevel & tiled_zoom_level, double bearing);
  QcViewportState(const QcViewportState & other);
  ~QcViewportState();

  QcViewportState & operator=(const QcViewportState & other);

  bool operator==(const QcViewportState & other) const;
  inline bool operator!=(const QcViewportState & other) const {
    return !operator==(other);
  }

  inline QcGeoCoordinateWGS84 wgs84() const { return m_coordinate.wgs84(); }
  inline QcGeoCoordinateWebMercator web_mercator() const { return m_coordinate.web_mercator(); }
  inline QcGeoCoordinatePseudoWebMercator pseudo_web_mercator() const { return m_coordinate; }
  // inline const QcGeoCoordinateNormalisedWebMercator & normalised_web_mercator() const { return m_coordinate.normalised_web_mercator(); }

  inline void set_coordinate(const QcGeoCoordinatePseudoWebMercator & coordinate) { m_coordinate = coordinate; }
  inline void set_coordinate(const QcGeoCoordinateWGS84 & coordinate) {
    m_coordinate = coordinate.pseudo_web_mercator();
  }
  inline void set_coordinate(const QcGeoCoordinateWebMercator & coordinate) {
    m_coordinate = coordinate.pseudo_web_mercator();
  }
  // inline void set_coordinate(const QcGeoCoordinateNormalisedWebMercator & coordinate) {
  //   m_coordinate = coordinate.pseudo_web_mercator();
  // }

  inline double bearing() const { return m_bearing; }
  inline void set_bearing(double bearing);

  inline const QcTiledZoomLevel & tiled_zoom_level() const { return m_tiled_zoom_level; }

  inline unsigned int zoom_level() const { return m_tiled_zoom_level.zoom_level(); }
  inline void set_zoom_level(unsigned int zoom_level) { m_tiled_zoom_level.set_zoom_level(zoom_level); }

 private:
  QcGeoCoordinatePseudoWebMercator m_coordinate;
  QcTiledZoomLevel m_tiled_zoom_level;
  double m_bearing;
};

/**************************************************************************************************/

class QC_EXPORT QcViewport : public QObject
{
  Q_OBJECT

 public:
  static QcInterval2DDouble interval_from_center_and_size(const QcVectorDouble & center, const QcVectorDouble & size);

 public:
  QcViewport(const QcViewportState & viewport_state, const QSize & viewport_size); // Fixme : tiled_zoom_level has parameters

  inline const QcViewportState & viewport_state() const { return m_state; }

  inline QcGeoCoordinateWGS84 wgs84() const { return m_state.wgs84(); }
  inline QcGeoCoordinateWebMercator web_mercator() const { return m_state.web_mercator(); }
  inline const QcGeoCoordinatePseudoWebMercator pseudo_web_mercator() const { return m_state.pseudo_web_mercator(); }
  // inline const QcGeoCoordinateNormalisedWebMercator & normalised_web_mercator() const { return m_state.normalised_web_mercator(); }

  // Fixme: zoom_factor versus resolution [m/px]
  inline double zoom_factor() const { return m_state.tiled_zoom_level().zoom_factor(); }
  inline unsigned int zoom_level() const { return m_state.tiled_zoom_level().zoom_level(); }

  inline double bearing() const { return m_state.bearing(); }

  inline QSize viewport_size() const { return m_viewport_size; }
  inline int width() const  { return m_viewport_size.width(); }
  inline int height() const  { return m_viewport_size.height(); }
  void set_viewport_size(const QSize & size);

  QcVectorDouble area_size_m() const { return m_area_size_m; }

  void set_coordinate(const QcGeoCoordinatePseudoWebMercator & coordinate);
  void set_coordinate(const QcGeoCoordinateWGS84 & coordinate) {
    set_coordinate(coordinate.pseudo_web_mercator());
  }
  void set_coordinate(const QcGeoCoordinateWebMercator & coordinate) {
    set_coordinate(coordinate.pseudo_web_mercator());
  }
  // void set_coordinate(const QcGeoCoordinateNormalisedWebMercator & coordinate) {
  //   set_coordinate(coordinate.pseudo_web_mercator());
  // }

  void set_zoom_level(unsigned int zoom_level);
  void set_zoom_factor(double zoom_factor);

  void set_bearing(double bearing);

  void stable_zoom(const QcVectorDouble & position_px, unsigned int zoom_level);
  void zoom_at(const QcGeoCoordinatePseudoWebMercator & coordinate, unsigned int zoom_level);
  void pan(const QcVectorDouble & translation);
  void pan(double x, double y) {
    pan(QcVectorDouble(x, y));
  }

  bool cross_date_line() const { return m_cross_date_line; }
  bool cross_west_line() const { return m_cross_west_line; }
  bool cross_east_line() const { return m_cross_east_line; }

  const QcPolygon & east_polygon() const { return m_east_polygon; }
  const QcInterval2DDouble & east_interval() const { return m_east_polygon.interval(); }
  const QcPolygon & middle_polygon() const { return m_middle_polygon; }
  const QcInterval2DDouble & middle_interval() const { return m_middle_polygon.interval(); }
  const QcPolygon & west_polygon() const { return m_west_polygon; }
  const QcInterval2DDouble & west_interval() const { return m_west_polygon.interval(); }

  QcGeoCoordinatePseudoWebMercator to_coordinate(const QcVectorDouble & position, bool clip_to_viewport) const;
  QcVectorDouble from_coordinate(const QcGeoCoordinatePseudoWebMercator & coordinate, bool clip_to_viewport) const;

  // QcGeoCoordinateWGS84 to_coordinate(const QcVectorDouble & position_px, bool clip_to_viewport) const {
  //   return to_coordinate(position_px, clip_to_viewport).wgs84();
  // }
  QcVectorDouble from_coordinate(const QcGeoCoordinateWGS84 & coordinate, bool clip_to_viewport) const {
    return from_coordinate(coordinate.pseudo_web_mercator(), clip_to_viewport);
  }

 signals:
  void viewport_changed();

 private:
  void update_area_size();
  QcVectorDouble inf_point() const;
  void update_area();

 private:
  QcViewportState m_state;
  QSize m_viewport_size; // Fixme: QcVectorInt ?
  QcVectorDouble m_area_size_m;
  QcVectorDouble m_half_diagonal_m;
  QcPolygon m_west_polygon;
  QcPolygon m_middle_polygon;
  QcPolygon m_east_polygon;
  bool m_cross_date_line;
  bool m_cross_west_line;
  bool m_cross_east_line;
};

/**************************************************************************************************/

#endif /* __VIEWPORT_H__ */

/***************************************************************************************************
 *
 * End
 *
 **************************************************************************************************/
