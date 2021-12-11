# this dockerfile needs to be changed according to this url
# https://dev.to/rogertorres/first-steps-with-docker-rust-30oi

FROM rust:1.49
COPY . .
RUN cargo build --release
# Run the binary
#CMD ["./"]