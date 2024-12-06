FROM nginx:alpine

# Install envsubst (part of gettext package)
RUN apk add --no-cache gettext

# Set working directory
WORKDIR /usr/share/nginx/html

# Copy the HTML template file
COPY index.html.template /usr/share/nginx/html/index.html.template

# Expose Nginx's default HTTP port
EXPOSE 80

# Replace environment variables and start Nginx
CMD envsubst '${WEBSOCKET_HOST} ${WEBSOCKET_PORT}' < /usr/share/nginx/html/index.html.template > /usr/share/nginx/html/index.html && nginx -g 'daemon off;'
