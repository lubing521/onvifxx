<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xs="http://www.w3.org/2001/XMLSchema"
	xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/">

<!-- No tranformation -->
<xsl:template match="@*|node()">
  <xsl:copy>
    <xsl:apply-templates select="@*|node()"/>
  </xsl:copy>
</xsl:template>

<!-- Change attribute value -->
<xsl:template match="@schemaLocation[parent::xs:import]">
  <xsl:attribute name="schemaLocation">
    <xsl:variable name="schemaUrl" select="."/>
    <xsl:choose>
      <xsl:when test="contains($schemaUrl, '/envelope/')">
        <xsl:value-of select="'envelope.xsd'"/>
      </xsl:when>
      <xsl:when test="contains($schemaUrl, '.xsd') = false">
        <xsl:call-template name="extractName">
          <xsl:with-param name="path" select="concat($schemaUrl, '.xsd')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="extractName">
          <xsl:with-param name="path" select="$schemaUrl"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:attribute>
</xsl:template>

<!-- Extract filename from path or URL -->
<xsl:template name="extractName">
  <xsl:param name="path"/>
  <xsl:choose>
    <xsl:when test="contains($path, '/')">
      <xsl:call-template name="extractName">
        <xsl:with-param name="path" select="substring-after($path,'/')"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$path"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>
</xsl:stylesheet>
